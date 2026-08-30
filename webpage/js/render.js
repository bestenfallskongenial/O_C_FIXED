		// initialize_helper();

		//	var compressor = initialize_compressor();

			// Greetings to Iq/RGBA! ;)

			var quality = 2, quality_levels = [ 1, 2, 4, 8 ];
			var toolbarUI = {}, galleryOverlay, textureGalleryOverlay, hardwareControlsOverlay, compileTimer, errorLines = [];
			var code, canvas, gl, buffer, currentProgram, vertexPosition,
			parameters = {
				startTime: Date.now(),
				time: 0,
				mouseX: 0.5,
				mouseY: 0.5,
				parAX: 0.5,
				parAY: 0.5,
				parAZ: 0.0,
				parAW: 0.0,
				parBX: 0.0,
				parBY: 0.0,
				parBZ: 0.0,
				parBW: 0.0,
				screenWidth: 0,
				screenHeight: 0
			},	
			hardwareMouseRouting = {
				xParamKey: 'parAX',
				yParamKey: 'parAY'
			},			
			bpmState = {
				lastButtonValue: null,
				lastButtonTime: 0,
				timeBuffer: [0, 0, 0, 0],
				intervalBuffer: [0, 0, 0],
				timeIndex: 0,
				resultBpm: 0,
				bufferInterval: 0,
				lastValidCalculation: 0
			},						
			surface = { centerX: 0, centerY: 0, width: 1, height: 1, isPanning: false, isZooming: false, lastX: 0, lastY: 0 },
			getWebGL, compileOnChangeCode = true;

			var dummyFunction = function() {};
			var seedState = Date.now() >>> 0;			

			init();

			if ( gl ) animate();

			function init() 
				{
				createCanvasAndToolbar();
				createCodeEditor();
				initGL();
				initTextureGallery();
				initHardwareControls();
				updateOverlayToggleStates();				
				setupInputHandlers();
				setupKeyboardShortcuts();
				
				onWindowResize();
				window.addEventListener( 'resize', onWindowResize, false );

				loadStaticShaderSources().then(function () 
					{
					loadShaderByIndex(1);

					hideCode();
					});
				}

			function nextSeedValue() {
				seedState = (Math.imul(1664525, seedState) + 1013904223) >>> 0;
				return seedState / 4294967295;
			}

				function htmlEncode(str)
				{

				return String(str)
					.replace(/&/g, '&amp;')
					.replace(/"/g, '&quot;')
					.replace(/'/g, '&#39;')
					.replace(/</g, '&lt;')
					.replace(/>/g, '&gt;');

				}