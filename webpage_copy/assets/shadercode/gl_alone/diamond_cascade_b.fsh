precision mediump float;

// Jade Echoes
//
// Diamonds repeatedly divide and nest along a horizontal seam. Their layers expand, contract, and split into smaller echoes, like folded shapes opening inside one another.
//
// par_a.x - shifts the first branch of the repeated diamond transformation horizontally.
// par_a.y - shifts the second branch horizontally.
// par_a.z - changes scaling in the first branch, altering nesting and repetition.
// par_a.w - changes scaling in the second branch, altering nesting and repetition.

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
		if(abs(p.x)+abs(p.y)<1.) {
		    r = cos(time+.3*i)/2.;
		    if(p.x<2.*r) {
			p.x -= r-par_a.x;
			p /= r+par_a.z;
		    }
		    else {
			p.x -= r+par_a.y;
			p /= r-par_a.w;
		    }
		    f = i/I;
		}
	}
    
	gl_FragColor = vec4(vec3(1.2, 1.5, 1.)*vec3(f), color.a);
}