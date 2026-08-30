function initGL() {
	try {
		gl = canvas.getContext('webgl', {
			antialias: false,
			depth: false,
			stencil: false,
			premultipliedAlpha: false,
			preserveDrawingBuffer: true
		});
	} catch (error) {}

	if (gl) {
		gl.getExtension('OES_standard_derivatives');

		buffer = gl.createBuffer();
		gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
		gl.bufferData(
			gl.ARRAY_BUFFER,
			new Float32Array([
				-1.0, -1.0, 1.0, 1.0,
				1.0, -1.0, 1.0, 1.0,
				-1.0, 1.0, 1.0, 1.0,
				1.0, -1.0, 1.0, 1.0,
				1.0, 1.0, 1.0, 1.0,
				-1.0, 1.0, 1.0, 1.0
			]),
			gl.STATIC_DRAW
		);

		surface.buffer = gl.createBuffer();
	} else {
		alert('WebGL not supported, but code will be shown.');
	}
}

function computeSurfaceCorners() {
	if (gl) {
		surface.width = surface.height * parameters.screenWidth / parameters.screenHeight;

		var halfWidth = surface.width * 0.5, halfHeight = surface.height * 0.5;

		gl.bindBuffer(gl.ARRAY_BUFFER, surface.buffer);
		gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
			surface.centerX - halfWidth, surface.centerY - halfHeight,
			surface.centerX + halfWidth, surface.centerY - halfHeight,
			surface.centerX - halfWidth, surface.centerY + halfHeight,
			surface.centerX + halfWidth, surface.centerY - halfHeight,
			surface.centerX + halfWidth, surface.centerY + halfHeight,
			surface.centerX - halfWidth, surface.centerY + halfHeight
		]), gl.STATIC_DRAW);
	}
}

function resetSurface() {
	surface.centerX = surface.centerY = 0;
	surface.height = 1;
	computeSurfaceCorners();
}

function onWindowResize() {
	canvas.width = window.innerWidth / quality;
	canvas.height = window.innerHeight / quality;

	parameters.screenWidth = canvas.width;
	parameters.screenHeight = canvas.height;

	computeSurfaceCorners();

	if (gl) {
		gl.viewport(0, 0, canvas.width, canvas.height);
		createRenderTargets();
	}
}