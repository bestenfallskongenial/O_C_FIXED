precision mediump float;

// Harmonic Ribbon
//
// A bundle of closely spaced wave curves stretches across the screen. Their intersections gather into bright knots while their outer edges fan into a fine woven ribbon, continually opening and tightening around the moving crests.
//
// par_a.x - increases horizontal wave frequency.
// par_a.y - increases the ribbon’s brightness and apparent thickness.
// par_a.z - compresses the ribbon vertically.

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

vec2 mouse = par_a.xy;

#define PI 1.
#define TWO_PI 9.0*PI
#define t time*0.3
#define MAX 30.
void main( void ) 

{
 // vec2 uv = tcoord;
	vec2 uv = (gl_FragCoord.xy - tres * 0.5) / max(tres.x, tres.y) * 4.0;
	uv *= 0.5;
	
	float e = 0.0;

    float amplitude = 5.0 + par_a.z * 10.0;  // Base 5.0, can get bigger with mouse.z
	
    for (float i=0.0;i<=16.0;i+=1.0) {
		e += 0.02*mouse.y/abs(cos(time + 3.0*mouse.x*i*uv.x) +amplitude*uv.y);
	}
	
	gl_FragColor = vec4(vec3(0.2, e, 1.0), color.a);	
}   