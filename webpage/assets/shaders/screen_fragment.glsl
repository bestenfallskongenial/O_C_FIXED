precision highp float;

uniform vec2 tres;
uniform sampler2D texture;

void main() {
	vec2 uv = gl_FragCoord.xy / tres.xy;
	gl_FragColor = texture2D(texture, uv);
}