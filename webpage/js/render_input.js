function setupInputHandlers() {
	if (gl) {
		var surfaceMouseDown = function (event) {
			if (event.shiftKey) {
				resetSurface();
			}
			if (event.button === 0) {
				surface.isPanning = true;
				document.body.style.cursor = 'move';
			} else {
				surface.isZooming = true;
				document.body.style.cursor = 'se-resize';
 
			}
			surface.lastX = event.clientX;
			surface.lastY = event.clientY;
			event.preventDefault();
		};

		var noContextMenu = function (event) {
			event.preventDefault();
		};

		canvas.addEventListener('mousedown', surfaceMouseDown, false);
		canvas.addEventListener('contextmenu', noContextMenu, false);
	}

	var clientXLast, clientYLast;

	document.addEventListener('pointermove', function (event) {
		var clientX = event.clientX;
		var clientY = event.clientY;

		if (clientXLast == clientX && clientYLast == clientY)
			return;

		clientXLast = clientX;
		clientYLast = clientY;

		stopHideUI();

		var dx, dy;

		var normalizedMouseX = clientX / window.innerWidth;
		var normalizedMouseY = clientY / window.innerHeight;
		parameters.mouseX = normalizedMouseX;
		parameters.mouseY = 1 - normalizedMouseY;

		if (hardwareMouseRouting && hardwareMouseRouting.xParamKey) {
			parameters[hardwareMouseRouting.xParamKey] = normalizedMouseX;
		}
		if (hardwareMouseRouting && hardwareMouseRouting.yParamKey) {
			parameters[hardwareMouseRouting.yParamKey] = normalizedMouseY;
		}

		if (surface.isPanning) {
			dx = clientX - surface.lastX;
			dy = clientY - surface.lastY;
			surface.centerX -= dx * surface.width / window.innerWidth;
			surface.centerY += dy * surface.height / window.innerHeight;
			surface.lastX = clientX;
			surface.lastY = clientY;
			computeSurfaceCorners();
			event.preventDefault();
		} else if (surface.isZooming) {
			dx = clientX - surface.lastX;
			dy = clientY - surface.lastY;
			surface.height *= Math.pow(0.997, dx + dy);
			surface.lastX = clientX;
			surface.lastY = clientY;
			computeSurfaceCorners();
			event.preventDefault();
		}
	}, false);

	function settleDown() {
		surface.isPanning = surface.isZooming = false;
		document.body.style.cursor = 'default';
 
	}

	function mouseLeave(event) {
		settleDown(event);

		if (!isCodeVisible())
			startHideUITimer();
	}

	document.addEventListener('mouseup', settleDown, false);
	document.addEventListener('mouseleave', mouseLeave, false);
}