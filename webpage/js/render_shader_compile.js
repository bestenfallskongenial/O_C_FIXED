function compile() {
	if (!gl) {
		if (!getWebGL) {
			getWebGL = true;
			toolbarUI.compileButton.addEventListener('click', function () {
				document.location = 'http://get.webgl.org/';
			}, false);
			toolbarUI.compileButton.title = 'http://get.webgl.org/';
			toolbarUI.compileButton.style.color = '#ff0000';
			toolbarUI.compileButton.textContent = 'WebGL not supported!';
			set_save_button('hidden');
		}
		return;
	}

	var program = gl.createProgram();
	var fragment = code.getValue();
	var vertex = shaderSources.surfaceVertex;

	var vs = createShader(vertex, gl.VERTEX_SHADER);
	var fs = createShader(fragment, gl.FRAGMENT_SHADER);

	if (vs == null || fs == null) return null;

	gl.attachShader(program, vs);
	gl.attachShader(program, fs);

	gl.deleteShader(vs);
	gl.deleteShader(fs);

	gl.linkProgram(program);

	if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
		showCode();

		var error = gl.getProgramInfoLog(program);

		toolbarUI.compileButton.title = error;
		console.error(error);

		console.error('VALIDATE_STATUS: ' + gl.getProgramParameter(program, gl.VALIDATE_STATUS), 'ERROR: ' + gl.getError());
		toolbarUI.compileButton.style.color = '#ff0000';
		toolbarUI.compileButton.textContent = 'errors';

		set_save_button('hidden');

		return;
	}

	if (currentProgram) {
		gl.deleteProgram(currentProgram);
	}

	currentProgram = program;

	toolbarUI.compileButton.style.color = '#00ff00';
	toolbarUI.compileButton.textContent = 'compiled';

	set_save_button('visible');

	cacheUniformLocation(program, 'time');
	cacheUniformLocation(program, 'mouse');
	cacheUniformLocation(program, 'tres');
	cacheUniformLocation(program, 'seed');
	cacheUniformLocation(program, 'audio');
	cacheUniformLocation(program, 'color');
	cacheUniformLocation(program, 'par_a');
	cacheUniformLocation(program, 'par_b');
	
	cacheUniformLocation(program, 'surfaceSize');
	cacheUniformLocation(program, 'tex_l');
	cacheUniformLocation(program, 'tex[0]');
	
	gl.useProgram(currentProgram);
	vertexPosition = gl.getAttribLocation(currentProgram, "vertex");

	gl.enableVertexAttribArray(vertexPosition);
}

function cacheUniformLocation(program, label) {
	if (program.uniformsCache === undefined) {
		program.uniformsCache = {};
	}
	program.uniformsCache[label] = gl.getUniformLocation(program, label);
}

function createShader(src, type) {
	var shader = gl.createShader(type);
	var line, lineNum, lineError, index = 0, indexEnd;

	while (errorLines.length > 0) {
		line = errorLines.pop();
		code.setLineClass(line, null);
		code.clearMarker(line);
	}

	gl.shaderSource(shader, src);
	gl.compileShader(shader);

	toolbarUI.compileButton.title = '';

	if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		showCode();

		var error = gl.getShaderInfoLog(shader);

		while ((error.length > 1) && (error.charCodeAt(error.length - 1) < 32)) {
			error = error.substring(0, error.length - 1);
		}

		toolbarUI.compileButton.title = error;
		console.error(error);

		toolbarUI.compileButton.style.color = '#ff0000';
		toolbarUI.compileButton.textContent = 'errors';

		set_save_button('hidden');

		while (index >= 0) {
			index = error.indexOf("ERROR: 0:", index);
			if (index < 0) { break; }
			index += 9;
			indexEnd = error.indexOf(':', index);
			if (indexEnd > index) {
				lineNum = parseInt(error.substring(index, indexEnd));
				if ((!isNaN(lineNum)) && (lineNum > 0)) {
					index = indexEnd + 1;
					indexEnd = error.indexOf("ERROR: 0:", index);
					lineError = htmlEncode((indexEnd > index) ? error.substring(index, indexEnd) : error.substring(index));
					line = code.setMarker(lineNum - 1, '<abbr title="' + lineError + '">' + lineNum + '</abbr>', "errorMarker");
					code.setLineClass(line, "errorLine");
					errorLines.push(line);
				}
			}
		}

		return null;
	}

	return shader;
}