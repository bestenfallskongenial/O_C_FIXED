function animate() {
	requestAnimationFrame(animate);
	render();
}

function render() {
	if (!currentProgram) return;

	parameters.time = Date.now() - parameters.startTime;

	toolbarUI.timeButton.textContent = parseTime(parameters.time);

	gl.useProgram(currentProgram);

	var clockTicks = getClockTicks();
	var manualTime = getManualTimeValue();
	var glTime = manualTime.useManual
		? manualTime.value
		: clockTicks / 1000000.0;

	gl.uniform1f(currentProgram.uniformsCache['time'], glTime);
	gl.uniform2f(currentProgram.uniformsCache['mouse'], parameters.mouseX, parameters.mouseY);
	gl.uniform2f(currentProgram.uniformsCache['tres'], parameters.screenWidth, parameters.screenHeight);

	setUniform4f(currentProgram.uniformsCache['seed'], nextSeedValue(), nextSeedValue(), nextSeedValue(), nextSeedValue());
	setUniform4f(currentProgram.uniformsCache['audio'], 0.0, 0.0, 0.0, 0.0);
	setUniform4f(currentProgram.uniformsCache['color'], 0.0, 0.0, 0.0, 1.0);
	setUniform4f(currentProgram.uniformsCache['par_a'], parameters.parAX, parameters.parAY, parameters.parAZ, parameters.parAW);
	setUniform4f(currentProgram.uniformsCache['par_b'], parameters.parBX, parameters.parBY, parameters.parBZ, parameters.parBW);
	updateTapTempo(clockTicks);
	updateHardwareControlValues();	

	gl.uniform2f(currentProgram.uniformsCache['surfaceSize'], surface.width, surface.height);
	updateTextureUniforms(currentProgram);
	
	gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
	gl.vertexAttribPointer(vertexPosition, 4, gl.FLOAT, false, 0, 0);

	gl.bindFramebuffer(gl.FRAMEBUFFER, null);

	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
	gl.drawArrays(gl.TRIANGLES, 0, 6);
}

function setUniform4f(location, x, y, z, w) {
	if (!location) {
		return;
	}
	gl.uniform4f(location, x, y, z, w);
}

function parseTime(ms) {
	const minutes = Math.floor(ms / 60000);
	const seconds = ((ms % 60000) / 1000).toFixed(2).padStart(5, '0');
	return `${minutes}:${seconds}`;
}

function updateHardwareControlValues() {
	if (!hardwareControlsOverlay) {
		return;
	}

	var valueMap = {
		'par_a.x': parameters.parAX,
		'par_a.y': parameters.parAY,
		'par_a.z': parameters.parAZ,
		'par_a.w': parameters.parAW,
		'par_b.x': parameters.parBX,
		'par_b.y': parameters.parBY,
		'par_b.z': parameters.parBZ,
		'par_b.w': parameters.parBW
	};

	var valueNodes = hardwareControlsOverlay.querySelectorAll('.hardware-control-value-text');
	valueNodes.forEach(function (node) {
		var key = node.dataset.valueKey;
		if (Object.prototype.hasOwnProperty.call(valueMap, key)) {
			node.textContent = valueMap[key].toFixed(2);
		}
	});
}

function getClockTicks() {
	if (typeof performance !== 'undefined' && typeof performance.now === 'function') {
		return performance.now() * 1000;
	}
	return Date.now() * 1000;
}

function getHardwareControl(controlId) {
	if (!hardwareControlsOverlay) {
		return null;
	}
	return hardwareControlsOverlay.querySelector('[data-control-id="' + controlId + '"]');
}

function getManualTimeValue() {
	var toggle = getHardwareControl('toggle_sq_5');
	var useManual = toggle && toggle.dataset.controlState === '1';
	if (!useManual) {
		return { useManual: false, value: 0 };
	}
	var scaled = parameters.parBX * 1024.0;
	var clamped = Math.min(Math.max(scaled, 0), 1023.0);
	return { useManual: true, value: clamped / 360.0 };
}

function updateTapTempo(clockTicks) {
	if (!bpmState) {
		return;
	}

	var button = getHardwareControl('button_rect_3');
	if (!button) {
		return;
	}

	var buttonValue = Number(button.dataset.controlValue || 0);
	if (buttonValue === bpmState.lastButtonValue) {
		return;
	}

	bpmState.lastButtonValue = buttonValue;
	var buttonTime = clockTicks;

	if (buttonTime === bpmState.lastButtonTime) {
		return;
	}

	bpmState.timeBuffer[bpmState.timeIndex] = buttonTime;
	bpmState.intervalBuffer[0] = bpmState.timeBuffer[1] - bpmState.timeBuffer[0];
	bpmState.intervalBuffer[1] = bpmState.timeBuffer[2] - bpmState.timeBuffer[1];
	bpmState.intervalBuffer[2] = bpmState.timeBuffer[3] - bpmState.timeBuffer[2];

	var interval0 = bpmState.intervalBuffer[0];
	var interval1 = bpmState.intervalBuffer[1];
	var interval2 = bpmState.intervalBuffer[2];

	if (interval0 > 0 && interval1 > 0 && interval2 > 0
		&& interval1 < interval0 * 1.25
		&& interval2 < interval0 * 1.25
		&& interval0 < interval2 * 1.25) {
		var averageInterval = (interval0 + interval1 + interval2) / 3;
		bpmState.resultBpm = 60000000 / averageInterval;
		bpmState.bufferInterval = averageInterval;
		bpmState.lastValidCalculation = clockTicks;
		updateBpmDisplay(bpmState.resultBpm);
	}

	bpmState.lastButtonTime = buttonTime;
	bpmState.timeIndex = (bpmState.timeIndex + 1) % 4;
}

function updateBpmDisplay(value) {
	if (!hardwareControlsOverlay) {
		return;
	}
	var control = getHardwareControl('value_rect_1');
	if (!control) {
		return;
	}
	var textNode = control.querySelector('.hardware-control-value-text');
	if (!textNode) {
		return;
	}
	textNode.textContent = value.toFixed(2);
}