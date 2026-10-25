precision mediump float;

// Luminous Brocade
//
// Nested grids unfold into glowing diamonds, circular pockets, and tightly repeated ornaments. Several pulsing layers pass through one another, making the surface resemble an intricate brocade whose threads continually rearrange around a central motif.

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

vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos( 6.28318*(c*t+d) );
}

void main(void) {
	vec2 uv = 2.0 * gl_FragCoord.xy / tres.xy - 1.0;
	uv.x /= tres.y / tres.x;
	vec2 uv0 = uv;
	vec3 finalCol = vec3(0.0);
	for (float i = 0.0; i < 3.0; i++) {
		uv = fract(1.0 * uv) - 0.5;
		if (i == 0.0) {
			uv *= 1.1;
		}
		uv.x = uv.x * (atan(time * 0.35) + 2.0);
		uv.y = uv.y * (atan(time * 0.4) + 2.0);
		float d = length(uv) * exp(-length(uv0));
		vec3 col = palette(length(uv0) + 0.3 * time);
		d = -d + 0.5 * d * d - 1.0 * d * d * d;
		if (mod(i, 2.0) != 0.0) {
			d = abs(sin(1.2 * (5.0 - i) * d + time) / 2.0);
		} else {
			d = abs(cos((5.0 - i) * d + 0.8 * time) / 2.0)*exp(-sin(-d));
		}
		d = 0.02 / d;
		d = smoothstep(0.1, 0.2, d);
		d = pow(d, 1.8);
		d = d / 5.0 * (5.0 - i);
		col *= d;
		finalCol += col;
		gl_FragColor = vec4(finalCol, color.a);	
	}
}