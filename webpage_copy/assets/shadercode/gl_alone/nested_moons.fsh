precision mediump float;

// Nested Moons
//
// Circular regions repeatedly divide and nest inside one another. Their layers expand, contract, and shift sideways, revealing smaller discs within larger ones like a procession of moons passing through overlapping windows.
//
// par_a.x - shifts the first branch of the repeated transformation horizontally.
// par_a.y - shifts the second branch horizontally.
// par_a.z - changes scaling in the first branch.
// par_a.w - changes scaling in the second branch.

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

void main( void ) {
	vec2 p = 0.9 * (gl_FragCoord.xy-tres/2.) / tres.y;
	float f=0., r;
    
	const float I = 16.;
	for(float i=1.; i<=I; i++) {
		if(length(p)<1.) {
		    r = cos(time+.3*i)/2.;
		    if(p.x<2.*r) {
			p.x -= par_a.x;
			p /= r+par_a.z;
		    }
		    else {
			p.x -= par_a.y;
			p /= r-par_a.w;
		    }
		    f = i/I;
		}
	}
    
	gl_FragColor = vec4(vec3(1.5, 1.2, 1.)*vec3(f), color.a);
}