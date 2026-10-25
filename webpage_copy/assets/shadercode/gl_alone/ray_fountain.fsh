precision mediump float;

// Ray Fountain
//
// Sweeping rays gather around a bright central knot, opening into curved fans and soft outward trails. Their different turning speeds continually reshape the arrangement, like a fountain whose streams pivot around the same luminous source.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:accent lines:
float f1 = mix(0.2, 0.8, par_a[1]);     //f1:spatial pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:squeeze:

vec2 mouse = par_a.xy;

float rand(int seed, float ray) {
	return mod(sin(float(seed)*1.0+ray*1.0)*1.0, 1.0);
}

void main( void ) {
	float pi = 3.14159265359;
	vec2 position = ( gl_FragCoord.xy / tres.xy )-vec2(0.5);
	position.y *= tres.y/tres.x;
	position.x = dot(position,position)*4.0;
	float ang = atan(position.y, position.x);
	float dist = length(position);
	gl_FragColor.rgb = vec3(0.5, 0.5, 0.5) * (pow(dist, -1.0) * 0.05);
	for (float ray = 0.0; ray < 12.0; ray += 1.0) {
		float rayang = rand(5234, ray)*6.2+time*5.0*(rand(2534, ray)-rand(3545, ray));
		rayang = mod(rayang, pi*2.0);
		if (rayang < ang - pi) {rayang += pi*2.0;}
		if (rayang > ang + pi) {rayang -= pi*2.0;}
		float brite = 0.3 - abs(ang - rayang);
		brite -= dist * 0.1;
		if (brite > 0.0) {
			gl_FragColor.rgb += vec3(sin(ray)+1.0, sin(ray*2.0)+1.0, sin(ray*4.0)+1.0) * brite;
		}
	}
	gl_FragColor.a = color.a;
}