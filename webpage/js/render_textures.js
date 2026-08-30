var textureSlots = [];
var textureMeta = [];
var textureFallback = null;
var textureUnitCount = 8;

function initTextureGallery() {
	textureGalleryOverlay = createTextureGalleryOverlay();
	initTextureSlots();
}

function createTextureGalleryOverlay() {
	var overlay = document.createElement('div');
	overlay.id = 'textureGallery';
	overlay.className = 'gallery-hidden';

	var grid = document.createElement('div');
	grid.className = 'texture-grid';

	for (var i = 0; i < textureUnitCount; i++) {
		grid.appendChild(createTextureTile(i));
	}

	overlay.appendChild(grid);

	var app = document.getElementById('app');
	app.appendChild(overlay);

	return overlay;
}

function createTextureTile(index) {
	var tile = document.createElement('button');
	tile.type = 'button';
	tile.className = 'texture-tile';
	tile.dataset.index = index;

	var label = document.createElement('span');
	label.className = 'texture-label';
	label.textContent = 'texture ' + (index + 1);
	tile.appendChild(label);

	var details = document.createElement('span');
	details.className = 'texture-details';
	details.textContent = 'empty';
	tile.appendChild(details);

	var input = document.createElement('input');
	input.type = 'file';
	input.accept = 'image/*';
	input.className = 'texture-input';
	input.dataset.index = index;

	tile.addEventListener('click', function () {
		input.click();
	});

	input.addEventListener('change', function (event) {
		var file = event.target.files[0];
		if (!file) {
			return;
		}
		loadTextureFromFile(file, index);
	});

	tile.appendChild(input);

	return tile;
}

function initTextureSlots() {
	textureSlots = new Array(textureUnitCount);
	textureMeta = new Array(textureUnitCount);

	for (var i = 0; i < textureUnitCount; i++) {
		textureSlots[i] = null;
		textureMeta[i] = {
			name: '',
			width: 0,
			height: 0,
			sourceUrl: '',
			loaded: false
		};
	}
}

function loadTextureFromFile(file, index) {
	validateTextureFile(file).then(function (result) {
		if (!result.valid) {
			updateTextureTileError(index, result.message);
			return;
		}
		return convertTextureFile(file);
	}).then(function (converted) {
		if (!converted) {
			return;
		}
		createOrUpdateTexture(index, converted.source, converted.width, converted.height);
		updateTextureTilePreview(index, file, converted.width, converted.height);
	});
}

function validateTextureFile(file) {
	// TODO: integrate ffmpeg-based validation/conversion (device-specific constraints).
	return Promise.resolve({
		valid: true,
		message: ''
	});
}

function convertTextureFile(file) {
	// TODO: swap in ffmpeg.wasm pipeline to produce device-ready 24-bit BMP output.
	if (window.createImageBitmap) {
		return createImageBitmap(file).then(function (bitmap) {
			return {
				source: bitmap,
				width: bitmap.width,
				height: bitmap.height
			};
		});
	}

	return new Promise(function (resolve) {
		var img = new Image();
		img.onload = function () {
			URL.revokeObjectURL(img.src);
			resolve({
				source: img,
				width: img.naturalWidth,
				height: img.naturalHeight
			});
		};
		img.src = URL.createObjectURL(file);
	});
}

function createOrUpdateTexture(index, source, width, height) {
	if (!gl) {
		return;
	}

	if (!textureSlots[index]) {
		textureSlots[index] = gl.createTexture();
	}

	gl.bindTexture(gl.TEXTURE_2D, textureSlots[index]);
	gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, source);

	textureMeta[index].width = width;
	textureMeta[index].height = height;
	textureMeta[index].loaded = true;
}

function updateTextureTilePreview(index, file, width, height) {
	var tile = textureGalleryOverlay.querySelector('.texture-tile[data-index="' + index + '"]');
	var details = tile.querySelector('.texture-details');
	var label = tile.querySelector('.texture-label');

	if (textureMeta[index].sourceUrl) {
		URL.revokeObjectURL(textureMeta[index].sourceUrl);
	}

	var url = URL.createObjectURL(file);
	textureMeta[index].sourceUrl = url;
	tile.style.backgroundImage = 'url("' + url + '")';
	tile.classList.add('texture-tile-loaded');
	label.textContent = file.name;
	details.textContent = width + '×' + height;
}

function updateTextureTileError(index, message) {
	var tile = textureGalleryOverlay.querySelector('.texture-tile[data-index="' + index + '"]');
	var details = tile.querySelector('.texture-details');
	tile.classList.add('texture-tile-error');
	details.textContent = message || 'invalid file';
}

function updateTextureUniforms(program) {
	if (!gl || !program) {
		return;
	}

	var samplerLocation = program.uniformsCache && program.uniformsCache['tex[0]'];
	var texCountLocation = program.uniformsCache && program.uniformsCache['tex_l'];

	if (!samplerLocation && !texCountLocation) {
		return;
	}

	ensureTextureFallback();

	var units = [];
	var loadedCount = 0;

	for (var i = 0; i < textureUnitCount; i++) {
		var texture = textureSlots[i] || textureFallback;
		gl.activeTexture(gl.TEXTURE0 + i);
		gl.bindTexture(gl.TEXTURE_2D, texture);
		units.push(i);
		if (textureMeta[i] && textureMeta[i].loaded) {
			loadedCount += 1;
		}
	}

	if (samplerLocation) {
		gl.uniform1iv(samplerLocation, units);
	}
	if (texCountLocation) {
		gl.uniform1i(texCountLocation, loadedCount);
	}
}

function ensureTextureFallback() {
	if (textureFallback || !gl) {
		return;
	}
	textureFallback = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, textureFallback);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
	gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
	gl.texImage2D(
		gl.TEXTURE_2D,
		0,
		gl.RGBA,
		1,
		1,
		0,
		gl.RGBA,
		gl.UNSIGNED_BYTE,
		new Uint8Array([0, 0, 0, 255])
	);
}