//hatsuyuki by Catzpaw 2016                   
//----------------------------------------// shader name length of 40 characters

precision mediump float;

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers
uniform int tex_l;                      // number of loaded texures

float f0 = mix(0.05, 0.95, par_a[0]);	//f0:stretch:
float f1 = mix(0.05, 0.95, par_a[1]);	//f1:beams:
float f2 = mix(0.05, 0.95, par_a[2]);	//f2:zigs:

vec2 translate(vec2 pos, vec2 translate)
{
	return pos + vec2(-translate.x, -translate.y);
}
 
mat2 rotate2D(float a)
{
	float co = cos(a), si = sin(a);
	return mat2(f0, -si, si, f1);
}
 
float sdBox(in vec2 p, in vec2 size)
{
	p = abs(p) - size;
	p *= rotate2D(1.0*time);
	return length(max(p, 0.0)) + min(0.0, max(p.x, p.y));
}
 
void main()
{
	highp vec2 size = vec2(tres.x, tres.y);
	vec2 uv = (2.0 * gl_FragCoord.xy - size) / size.x;
	uv = translate(uv, vec2(0.0));
	uv *= rotate2D(0.5*time);
	highp vec3 col_out = vec3(0.0);
	float box = sdBox(uv,  vec2(0.6));
	float box1 = sdBox(uv, vec2(0.5));
	float box2 = sdBox(uv, vec2(0.4));
	float box3 = sdBox(uv, vec2(0.3));
	float box4 = sdBox(uv, vec2(0.2));
	float box5 = sdBox(uv, vec2(0.1));
	float aaWidth = ( 10.0 * f2 ) /size.x;
	col_out = mix(col_out, vec3(0.9), smoothstep(aaWidth, aaWidth-0.01, abs(box)));
	col_out = mix(col_out, vec3(0.7), smoothstep(aaWidth, aaWidth-0.01, abs(box1)));
	col_out = mix(col_out, vec3(0.5), smoothstep(aaWidth, aaWidth-0.01, abs(box2)));
	col_out = mix(col_out, vec3(0.4), smoothstep(aaWidth, aaWidth-0.01, abs(box3)));
	col_out = mix(col_out, vec3(0.2), smoothstep(aaWidth, aaWidth-0.01, abs(box4)));
	col_out = mix(col_out, vec3(0.1), smoothstep(aaWidth, aaWidth-0.01, abs(box5)));

	gl_FragColor = vec4(col_out, color.a);
}