function createCanvasAndToolbar() {
	canvas = createCanvas();

	var toolbarContainers = createToolbarContainers();
	var toolbarControls = createToolbarControls(toolbarContainers);
	wireToolbarHandlers(toolbarControls);

	galleryOverlay = document.getElementById('gallery');

	toolbarUI = {
		root: toolbarContainers.root,
		left: toolbarContainers.left,
		right: toolbarContainers.right,
		downloadButton: toolbarControls.downloadButton,
		screenshotButton: toolbarControls.screenshotButton,		
		fullscreenButton: toolbarControls.fullscreenButton,
		backLink: toolbarControls.backLink,
		showButton: toolbarControls.showButton,
		galleryButton: toolbarControls.galleryButton,
		timeButton: toolbarControls.timeButton,
		textureButton: toolbarControls.textureButton,	
		hardwareControlsButton: toolbarControls.hardwareControlsButton,		
		qualitySelect: toolbarControls.qualitySelect,
		compileButton: toolbarControls.compileButton
	};

}

function createCanvas() {
	var canvasElement = document.createElement('canvas');
	var main = document.getElementById("main");
main.appendChild(canvasElement);
	return canvasElement;
}

function createToolbarContainers() {
	var toolbarRoot = document.createElement('div');
	toolbarRoot.id = 'toolbar';

	var toolbarLeft = document.createElement('div');
	toolbarLeft.className = 'toolbar-left';

	var toolbarRight = document.createElement('div');
	toolbarRight.className = 'toolbar-right';

	toolbarRoot.appendChild(toolbarLeft);
	toolbarRoot.appendChild(toolbarRight);

	document.getElementById("main").appendChild(toolbarRoot);

	return {
		root: toolbarRoot,
		left: toolbarLeft,
		right: toolbarRight
	};
}

function createToolbarControls(containers) {
	var downloadButton = createToolbarButton('download', 'downloadShaderButton');
	downloadButton.title = 'press Ctrl+D';
	containers.right.appendChild(downloadButton);

	var screenshotButton = createToolbarButton('screenshot', 'downloadScreenshotButton');
	screenshotButton.title = 'press Ctrl+S';
	containers.right.appendChild(screenshotButton);

	var fullscreenButton = createToolbarImage('../img/fullscreen.svg', 'Press F11 to enter or leave fullscreen mode');
	containers.right.appendChild(fullscreenButton);

	var backLink = createToolbarLink('back', '/index.html');
	containers.right.appendChild(backLink);

	var showButton = createToolbarButton('code');
	showButton.title = 'press C';
	containers.left.appendChild(showButton);

	var galleryButton = createToolbarButton('gallery');
	galleryButton.title = 'press P';
	containers.left.appendChild(galleryButton);

	var textureButton = createToolbarButton('textures');
	textureButton.title = 'press T';
	containers.left.appendChild(textureButton);

	var hardwareControlsButton = createToolbarButton('gui');
	hardwareControlsButton.title = 'press G';
	containers.left.appendChild(hardwareControlsButton);

	var timeButton = createToolbarButton('0:00.00');
	containers.left.appendChild(timeButton);

	var qualitySelect = createQualitySelect();
	containers.left.appendChild(qualitySelect);

	var compileButton = createToolbarButton('compiled');
	containers.left.appendChild(compileButton);

	return {
		downloadButton: downloadButton,
		screenshotButton: screenshotButton,		
		fullscreenButton: fullscreenButton,
		backLink: backLink,
		showButton: showButton,
		galleryButton: galleryButton,
		textureButton: textureButton,		
		hardwareControlsButton: hardwareControlsButton,		
		timeButton: timeButton,
		qualitySelect: qualitySelect,
		compileButton: compileButton
	};
}

function wireToolbarHandlers(controls) {
	controls.downloadButton.addEventListener("click", function () {
		var shaderSource = code.getValue();

		var blob = new Blob(
			[shaderSource],
			{ type: "text/plain;charset=utf-8" }
		);

		var url = URL.createObjectURL(blob);

		triggerDownload(url, "shader.frag");

		URL.revokeObjectURL(url);
	});

	controls.screenshotButton.addEventListener("click", function () {
		downloadCanvasScreenshot();
	});

	controls.fullscreenButton.addEventListener('click', function () {
		if (document.fullscreenElement) {
			document.exitFullscreen();
			return;
		} else if (document.webkitFullscreenElement) {
			document.webkitExitFullscreen();
			return;
		}

		if (document.documentElement.requestFullscreen) {
			document.documentElement.requestFullscreen({ navigationUI: 'hide' });
		} else if (document.documentElement.webkitRequestFullscreen) {
			document.documentElement.webkitRequestFullscreen();
		}
	}, false);

	controls.showButton.addEventListener('click', function () {
		isCodeVisible() ? hideCode() : showCode();
	}, false);
	controls.galleryButton.addEventListener('click', function () {
		isGalleryVisible() ? hideGallery() : showGallery();
	}, false);
	controls.textureButton.addEventListener('click', function () {
		isTextureGalleryVisible() ? hideTextureGallery() : showTextureGallery();
	}, false);
	controls.hardwareControlsButton.addEventListener('click', function () {
		isHardwareControlsVisible() ? hideHardwareControls() : showHardwareControls();
	}, false);

	controls.timeButton.addEventListener('click', function () {
		parameters.startTime = Date.now();
	}, false);
	controls.qualitySelect.addEventListener('change', function (event) {
		quality = quality_levels[event.target.selectedIndex];
		onWindowResize();
	}, false);

	controls.compileButton.addEventListener('click', function () {
		compile();
	}, false);
}

function downloadCanvasScreenshot() {
	if (!canvas) {
		return;
	}

	var filename = "shader-screenshot.jpg";
	if (canvas.toBlob) {
		canvas.toBlob(function (blob) {
			if (!blob) {
				return;
			}
			var url = URL.createObjectURL(blob);
			triggerDownload(url, filename);
			URL.revokeObjectURL(url);
		}, "image/jpeg", 0.92);
		return;
	}

	var dataUrl = canvas.toDataURL("image/jpeg", 0.92);
	triggerDownload(dataUrl, filename);
}

function triggerDownload(url, filename) {
	var link = document.createElement("a");
	link.href = url;
	link.download = filename;
	document.body.appendChild(link);
	link.click();
	document.body.removeChild(link);
}

function createToolbarButton(text, id) {
	var button = document.createElement('button');
	if (id) {
		button.id = id;
	}
	button.textContent = text;
	return button;
}

function createToolbarLink(text, href) {
	var link = document.createElement('a');
	link.textContent = text;
	link.href = href;
	return link;
}

function createToolbarImage(src, title) {
	var image = document.createElement('img');
	image.src = src;
	image.title = title;
	return image;
}

function createQualitySelect() {
var select = document.createElement('select');

	for (var i = 0; i < quality_levels.length; i++) {
		var option = document.createElement('option');
		option.textContent = quality_levels[i] + 'x';
		if (quality_levels[i] == quality) option.selected = true;
		select.appendChild(option);
	}

return select;
}

function createCodeEditor() {
	code = CodeMirror(document.getElementById("main"), {
		lineNumbers: true,
		matchBrackets: true,
		indentWithTabs: true,
		tabSize: 8,
		indentUnit: 8,
		mode: "text/x-glsl",
		onChange: function () {
			if (compileOnChangeCode) {
				clearTimeout(compileTimer);
				compileTimer = setTimeout(compile, 500);
			}
		}
	});
	code.getWrapperElement().style.visibility = 'hidden';
	if (toolbarUI && toolbarUI.compileButton) {
		toolbarUI.compileButton.style.visibility = 'hidden';
	}
	set_save_button('hidden');
	set_parent_button('hidden');
}

var overlayPolicy = {
	code: { hides: 		[ 			'gallery', 	'textures', 'gui' ] },
	gallery: { hides: 	[ 'code', 				'textures', 'gui' ] },
	textures: { hides: 	[ 'code', 	'gallery', 				'gui' ] },
	gui: { hides: 		[ 'code', 	'gallery', 	'textures'		  ] }
};

function enforceOverlayPolicy(activeOverlay) {
	var activePolicy = overlayPolicy[activeOverlay] || {};
	var hides = activePolicy.hides || [];
	var overlays = {
		code: { hide: hideCode, isVisible: isCodeVisible },
		gallery: { hide: hideGallery, isVisible: isGalleryVisible },
		textures: { hide: hideTextureGallery, isVisible: isTextureGalleryVisible },
		gui: { hide: hideHardwareControls, isVisible: isHardwareControlsVisible }
	};

	hides.forEach(function (key) {
		if (key === activeOverlay || !overlays[key]) {
			return;
		}
		if (overlays[key].isVisible()) {
			overlays[key].hide();
		}
	});
}

function setToolbarToggleState(button, isActive) {
	if (!button) {
		return;
	}
	button.classList.toggle('toolbar-toggle-active', isActive);
}

function updateOverlayToggleStates() {
	setToolbarToggleState(toolbarUI.showButton, isCodeVisible());
	setToolbarToggleState(toolbarUI.galleryButton, isGalleryVisible());
	setToolbarToggleState(toolbarUI.textureButton, isTextureGalleryVisible());
	setToolbarToggleState(toolbarUI.hardwareControlsButton, isHardwareControlsVisible());
}

function showCode() {
	enforceOverlayPolicy('code');
	setToolbarToggleState(toolbarUI.showButton, true);
	code.getWrapperElement().style.visibility = 'visible';
	toolbarUI.compileButton.style.visibility = 'visible';
	set_save_button('visible');
	set_parent_button('visible');
}

function hideCode() {
	setToolbarToggleState(toolbarUI.showButton, false);
	code.getWrapperElement().style.visibility = 'hidden';
	toolbarUI.compileButton.style.visibility = 'hidden';
	set_save_button('hidden');
	set_parent_button('hidden');
	stopHideUI();
}

function isCodeVisible() {
	return code && code.getWrapperElement().style.visibility === 'visible';
}

function showGallery() {
	enforceOverlayPolicy('gallery');
	setToolbarToggleState(toolbarUI.galleryButton, true);
	galleryOverlay.classList.remove('gallery-hidden');
}

function hideGallery() {
	setToolbarToggleState(toolbarUI.galleryButton, false);
	galleryOverlay.classList.add('gallery-hidden');
}

function isGalleryVisible() {
	return !galleryOverlay.classList.contains('gallery-hidden');
}

function showTextureGallery() {
	enforceOverlayPolicy('textures');
	setToolbarToggleState(toolbarUI.textureButton, true);
	textureGalleryOverlay.classList.remove('gallery-hidden');
}

function hideTextureGallery() {
	setToolbarToggleState(toolbarUI.textureButton, false);
	textureGalleryOverlay.classList.add('gallery-hidden');
}

function isTextureGalleryVisible() {
	return textureGalleryOverlay && !textureGalleryOverlay.classList.contains('gallery-hidden');
}

function showHardwareControls() {
	enforceOverlayPolicy('gui');
	setToolbarToggleState(toolbarUI.hardwareControlsButton, true);
	hardwareControlsOverlay.classList.remove('gallery-hidden');
}

function hideHardwareControls() {
	setToolbarToggleState(toolbarUI.hardwareControlsButton, false);
	hardwareControlsOverlay.classList.add('gallery-hidden');
}

function isHardwareControlsVisible() {
	return hardwareControlsOverlay && !hardwareControlsOverlay.classList.contains('gallery-hidden');
}

function setupKeyboardShortcuts() {
	window.addEventListener('keydown', function (event) {
		if (event.defaultPrevented) {
			return;
		}
		if (event.metaKey || event.altKey) {
			return;
		}
		var target = event.target;
		if (target && (target.tagName === 'INPUT' || target.tagName === 'TEXTAREA' || target.tagName === 'SELECT' || target.isContentEditable)) {
			return;
		}
		var key = event.key;
		if (!key) {
			return;
		}
		switch (key.toLowerCase()) {
			case 'p':
				if (event.ctrlKey) {
					return;
				}
				isGalleryVisible() ? hideGallery() : showGallery();
				break;
			case 't':
				if (event.ctrlKey) {
					return;
				}
				isTextureGalleryVisible() ? hideTextureGallery() : showTextureGallery();
				break;
			case 'c':
				if (event.ctrlKey) {
					return;
				}
				isCodeVisible() ? hideCode() : showCode();
				break;
			case 'g':
				if (event.ctrlKey) {
					return;
				}
				isHardwareControlsVisible() ? hideHardwareControls() : showHardwareControls();
				break;				
			case 's':
				if (!event.ctrlKey) {
					return;
				}
				event.preventDefault();
				downloadCanvasScreenshot();
				break;
			case 'd':
				if (!event.ctrlKey) {
					return;
				}
				event.preventDefault();
				var shaderSource = code.getValue();
				var blob = new Blob(
					[shaderSource],
					{ type: "text/plain;charset=utf-8" }
				);
				var url = URL.createObjectURL(blob);
				triggerDownload(url, "shader.frag");
				URL.revokeObjectURL(url);
				break;
			default:
				break;
		}
	});
}


var hideUITimer;
var isUIHidden = false;

function startHideUITimer() {
	stopHideUITimer();
	if (!isUIHidden && !isCodeVisible())
		hideUITimer = window.setTimeout(onHideUITimer, 1000 * 3);

	function onHideUITimer() {
		stopHideUITimer();
		if (!isUIHidden && !isCodeVisible()) {
			isUIHidden = true;
			toolbarUI.root.style.opacity = '0';
			document.body.style.cursor = 'none';
		}
	}

	function stopHideUITimer() {
		if (hideUITimer) {
			window.clearTimeout(hideUITimer);
			hideUITimer = 0;
		}
	}
}

function stopHideUI() {
	if (isUIHidden) {
		isUIHidden = false;
		toolbarUI.root.style.opacity = '1';
		document.body.style.cursor = '';
	}
	startHideUITimer();
}