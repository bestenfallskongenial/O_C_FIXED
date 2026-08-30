// helpers.js — utilities + shader loader
var shaderSources = {

	surfaceVertex: ""
};
var shaderCache = {};

function loadStaticShaderSources() {
	var shaderFiles = [

		{ key: "surfaceVertex", path: "/assets/shaders/surface_vertex.glsl" }
	];

	return Promise.all(
		shaderFiles.map(function (shaderFile) {
			return fetch(shaderFile.path)
				.then(function (res) {
					if (!res.ok) throw new Error("fetch failed");
					return res.text();
				})
				.then(function (text) {
					shaderSources[shaderFile.key] = text;
				});
		})
	);
}
// canonical shader loader (replaces load_url_code / readURL / setURL)
function loadShaderByIndex(index) {
	var s = String(index).padStart(2, "0");
	var path = "/assets/shaders/shader" + s + ".fsh";

	function applyShaderSource(text) {
		compileOnChangeCode = false;
		code.setValue(text);
		compile();
		compileOnChangeCode = true;
	}

	fetch(path)
		.then(function (res) {
			if (!res.ok) throw new Error("fetch failed");
			return res.text();
		})
		.then(function (text) {
			shaderCache[path] = text;
			applyShaderSource(text);
		})
		.catch(function (err) {
			if (shaderCache[path]) {
				console.warn("Shader load failed, using cached:", path, err);
				applyShaderSource(shaderCache[path]);
				return;
			}
			console.error("Shader load failed:", path, err);
		});
}
// --- stubs kept for render.js compatibility ---
function set_save_button(visibility) {}
function set_parent_button(visibility) {}
// function add_server_buttons() {}
