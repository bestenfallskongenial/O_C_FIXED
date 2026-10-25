var HARDWARE_BUTTON_MODE_LABELS = ['adc', 'bmp', 'trg', 'pot', 'lf1', 'lf2'];

var HARDWARE_CIRCLE_PARAM_BY_ID = {
	button_circle_1: 'parAX',
	button_circle_2: 'parAY',
	button_circle_3: 'parAZ',
	button_circle_4: 'parAW',
	button_circle_5: 'parBX',
	button_circle_6: 'parBY',
	button_circle_7: 'parBZ',
	button_circle_8: 'parBW'
};

function initHardwareControls() {
	hardwareControlsOverlay = createHardwareControlsOverlay();
}

function createHardwareControlsOverlay() {
	var overlay = document.createElement('div');
	overlay.id = 'hardwareControls';
	overlay.className = 'gallery-hidden';

	overlay.appendChild(createHardwareRow([
		createHardwareValue('par_a.x'),		
		createHardwareControl('hardware-slider', 'slider_h_1', 'parAX'),
		createHardwareControl('hardware-toggle', 'toggle_sq_1'),
		createHardwareControl('hardware-button-circle', 'button_circle_1'),
		createHardwareControl('hardware-button-circle', 'button_circle_2'),
		createHardwareControl('hardware-toggle', 'toggle_sq_2'),
		createHardwareControl('hardware-slider', 'slider_h_2', 'parAY'),
		createHardwareValue('par_a.y')
	]));

	overlay.appendChild(createHardwareRow([
		createHardwareValue('par_a.z'),		
		createHardwareControl('hardware-slider', 'slider_h_3', 'parAZ'),
		createHardwareControl('hardware-toggle', 'toggle_sq_3'),
		createHardwareControl('hardware-button-circle', 'button_circle_3'),
		createHardwareControl('hardware-button-circle', 'button_circle_4'),
		createHardwareControl('hardware-toggle', 'toggle_sq_4'),
		createHardwareControl('hardware-slider', 'slider_h_4', 'parAW'),
		createHardwareValue('par_a.w')
	]));

	overlay.appendChild(createHardwareRow([
		createHardwareControl('hardware-toggle', 'button_circle_9')
	]));

	overlay.appendChild(createHardwareRow([
		createHardwareControl('hardware-toggle', 'button_circle_10')
	]));

	overlay.appendChild(createHardwareRow([
		createHardwareValue('par_b.x'),		
		createHardwareControl('hardware-slider', 'slider_h_5', 'parBX'),
		createHardwareControl('hardware-toggle', 'toggle_sq_5'),
		createHardwareControl('hardware-button-circle', 'button_circle_5'),
		createHardwareControl('hardware-button-circle', 'button_circle_6'),
		createHardwareControl('hardware-toggle', 'toggle_sq_6'),
		createHardwareControl('hardware-slider', 'slider_h_6', 'parBY'),
		createHardwareValue('par_b.y')
	]));

	overlay.appendChild(createHardwareRow([
		createHardwareValue('par_b.z'),		
		createHardwareControl('hardware-slider', 'slider_h_7', 'parBZ'),
		createHardwareControl('hardware-toggle', 'toggle_sq_7'),
		createHardwareControl('hardware-button-circle', 'button_circle_7'),
		createHardwareControl('hardware-button-circle', 'button_circle_8'),
		createHardwareControl('hardware-toggle', 'toggle_sq_8'),
		createHardwareControl('hardware-slider', 'slider_h_8', 'parBW'),
		createHardwareValue('par_b.w')
	]));

//	overlay.appendChild(createHardwareSpacer());

	overlay.appendChild(createHardwareRow([
		createHardwareControl('hardware-button-rect', 'button_rect_1'),
		createHardwareControl('hardware-button-rect', 'button_rect_2')
	]));

	overlay.appendChild(createHardwareRow([
		createHardwareControl('hardware-button-rect', 'button_rect_3'),
		createHardwareControl('hardware-value-rect', 'value_rect_1')
	]));

	var app = document.getElementById('app');
	app.appendChild(overlay);

	return overlay;
}

function createHardwareRow(controls) {
	var row = document.createElement('div');
	row.className = 'hardware-row';
	controls.forEach(function (control) {
		row.appendChild(control);
	});
	return row;
}

function createHardwareSpacer() {
	var spacer = document.createElement('div');
	spacer.className = 'hardware-spacer';
	return spacer;
}

function createHardwareControl(className, label, paramKey) {
	var control;
	if (className === 'hardware-slider') {
		control = document.createElement('input');
		control.type = 'range';
		control.min = '0';
		control.max = '1';
		control.step = '0.01';
		control.value = '0';
		control.addEventListener('input', function () {
			control.dataset.controlValue = String(control.value);
			if (handleHardwareSliderModeRouting(control)) {
				return;
			}			
			if (control.dataset.paramKey) {
				var numericValue = Number(control.value);
				if (!Number.isNaN(numericValue)) {
					parameters[control.dataset.paramKey] = numericValue;
				}
			}			
		});
		control.dataset.controlValue = String(control.value);
	} else if (className === 'hardware-toggle') {
		control = document.createElement('input');
		control.type = 'checkbox';
		control.checked = false;
		control.addEventListener('change', function () {
			control.dataset.controlState = control.checked ? '1' : '0';
			if (control.checked) {
				enforceHardwareModeToggleExclusivity(control);
			}			
		});
		control.dataset.controlState = '0';
	} else if (className === 'hardware-button-circle' || className === 'hardware-button-rect') {
		control = document.createElement('button');
		control.type = 'button';
		control.dataset.controlValue = '0';
		control.dataset.controlMax = '5';
		if (className === 'hardware-button-circle') {
			control.dataset.modeMax = String(HARDWARE_BUTTON_MODE_LABELS.length);
			control.dataset.modeIndex = '0';
			applyHardwareButtonModeLabel(control, 0);
		}		
		control.addEventListener('click', function () {
			if (className === 'hardware-button-circle') {
				return;
			}			
			var maxValue = Number(control.dataset.controlMax || 5);
			var currentValue = Number(control.dataset.controlValue || 0);
			var nextValue = (currentValue + 1) % (maxValue + 1);
			control.dataset.controlValue = String(nextValue);
//			if (className === 'hardware-button-circle') {
//				setHardwareCircleModeIndex(control, nextValue);
//			}			
		});

		if (className === 'hardware-button-circle') {
			control.addEventListener('mousedown', function (event) {
				handleHardwareCircleMouseRouting(event, control);
			});
			control.addEventListener('contextmenu', function (event) {
				event.preventDefault();
			});
		}		
	} else if (className === 'hardware-value-rect') {
		control = document.createElement('div');
		var valueText = document.createElement('span');
		valueText.className = 'hardware-control-value-text';
		valueText.textContent = '0.00';
		control.appendChild(valueText);
	} else {
		control = document.createElement('div');
	}

	control.className = 'hardware-control ' + className;
	control.dataset.controlId = label;
	if (paramKey) {
		control.dataset.paramKey = paramKey;
	}	
	control.setAttribute('aria-label', label);
	applyHardwareControlFlag(control);

	return control;
}

function handleHardwareCircleMouseRouting(event, control) {
	if (!event || !control || !control.dataset) {
		return;
	}

	var paramKey = HARDWARE_CIRCLE_PARAM_BY_ID[control.dataset.controlId];
	if (!paramKey || !hardwareMouseRouting) {
		return;
	}

	if (event.button === 0) {
		hardwareMouseRouting.xParamKey = paramKey;
		event.preventDefault();
	} else if (event.button === 2) {
		hardwareMouseRouting.yParamKey = paramKey;
		event.preventDefault();
	}
}

function enforceHardwareModeToggleExclusivity(activeToggle) {
	if (!hardwareControlsOverlay || !activeToggle || !activeToggle.dataset) {
		return;
	}

	var activeToggleId = activeToggle.dataset.controlId;
	if (activeToggleId !== 'button_circle_9' && activeToggleId !== 'button_circle_10') {
		return;
	}

	var otherToggleId = activeToggleId === 'button_circle_9' ? 'button_circle_10' : 'button_circle_9';
	var otherToggle = hardwareControlsOverlay.querySelector('[data-control-id="' + otherToggleId + '"]');
	if (!otherToggle) {
		return;
	}

	otherToggle.checked = false;
	otherToggle.dataset.controlState = '0';
}

function handleHardwareSliderModeRouting(sliderControl) {
	if (!hardwareControlsOverlay) {
		return false;
	}

	var sliderControlId = sliderControl.dataset.controlId || '';
	var sliderMatch = sliderControlId.match(/^slider_h_(\d+)$/);
	if (!sliderMatch) {
		return false;
	}

	var sliderIndex = Number(sliderMatch[1]);
	if (!Number.isFinite(sliderIndex)) {
		return false;
	}

	var groupUsesModeRouting = (sliderIndex >= 1 && sliderIndex <= 4 && isHardwareModeToggleEnabled('button_circle_9'))
		|| (sliderIndex >= 5 && sliderIndex <= 8 && isHardwareModeToggleEnabled('button_circle_10'));

	// button_circle_9 and button_circle_10 are mutually exclusive edit gates.
	if (!groupUsesModeRouting) {
		return false;
	}

	var circleControlId = 'button_circle_' + sliderIndex;
	var circleControl = hardwareControlsOverlay.querySelector('[data-control-id="' + circleControlId + '"]');
	if (!circleControl) {
		return false;
	}

	var sliderValue = Number(sliderControl.value);
	if (Number.isNaN(sliderValue)) {
		return true;
	}

	var modeCount = Number(circleControl.dataset.modeMax || HARDWARE_BUTTON_MODE_LABELS.length);
	if (!Number.isFinite(modeCount) || modeCount < 1) {
		modeCount = 1;
	}

	var modeIndex = Math.floor(sliderValue * modeCount);
	if (modeIndex >= modeCount) {
		modeIndex = modeCount - 1;
	}
	if (modeIndex < 0) {
		modeIndex = 0;
	}

	setHardwareCircleModeIndex(circleControl, modeIndex);
	return true;
}


function isHardwareModeToggleEnabled(toggleId) {
	if (!hardwareControlsOverlay) {
		return false;
	}
	var modeToggle = hardwareControlsOverlay.querySelector('[data-control-id="' + toggleId + '"]');
	return Boolean(modeToggle && modeToggle.dataset.controlState === '1');
}

function setHardwareCircleModeIndex(circleControl, modeIndex) {
	var modeCount = Number(circleControl.dataset.modeMax || HARDWARE_BUTTON_MODE_LABELS.length);
	if (!Number.isFinite(modeCount) || modeCount < 1) {
		modeCount = 1;
	}

	var clampedIndex = Math.floor(Number(modeIndex));
	if (!Number.isFinite(clampedIndex)) {
		clampedIndex = 0;
	}
	if (clampedIndex < 0) {
		clampedIndex = 0;
	}
	if (clampedIndex >= modeCount) {
		clampedIndex = modeCount - 1;
	}

	circleControl.dataset.modeIndex = String(clampedIndex);
	circleControl.dataset.controlValue = String(clampedIndex);
	circleControl.dataset.controlMax = String(modeCount - 1);
	applyHardwareButtonModeLabel(circleControl, clampedIndex);
}

function applyHardwareButtonModeLabel(circleControl, modeIndex) {
	var label = HARDWARE_BUTTON_MODE_LABELS[modeIndex] || String(modeIndex);
	circleControl.textContent = label;
}

function applyHardwareControlFlag(control) {
	if (!control || !control.dataset || !control.dataset.controlId) {
		return;
	}

	var controlId = control.dataset.controlId;
	var rootStyle = getComputedStyle(document.documentElement);
	var enabledRaw = rootStyle.getPropertyValue('--hardware-control-' + controlId + '-enabled').trim();
	if (!enabledRaw) {
		return;
	}

	var isEnabled = enabledRaw !== '0';
	if (isEnabled) {
		return;
	}

	control.classList.add('hardware-control-disabled');
	if ('disabled' in control) {
		control.disabled = true;
	}
	if (control.tagName === 'INPUT' && control.type === 'checkbox') {
		control.checked = false;
		control.dataset.controlState = '0';
	}
	control.setAttribute('aria-hidden', 'true');
	control.setAttribute('tabindex', '-1');
}

function createHardwareValue(valueKey) {
	var control = document.createElement('div');
	control.className = 'hardware-control hardware-control-value';

	var text = document.createElement('span');
	text.className = 'hardware-control-value-text';
	text.dataset.valueKey = valueKey;
	text.textContent = '0.00';
	control.appendChild(text);

	return control;
}