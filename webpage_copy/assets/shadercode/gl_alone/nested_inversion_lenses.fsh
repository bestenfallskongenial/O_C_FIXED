precision mediump float;

// Nested Inversion Lenses
//
// Repeated circular inversions gather the image into nested lenses and curled lobes. Fine contour bands wrap around their edges while the structure slowly changes shape, like small worlds folding into one another.
//
// par_a.x - cross-axis mixing in each repeated transformation, changing the curl and orientation.
// par_a.y - scale multiplier between successive inversions.
// par_a.z - strength of the contour modulation, increasing band density.

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

/*
 * inspired by http://www.fractalforums.com/new-theories-and-research/very-simple-formula-for-fractal-patterns/
 * a slight(?) different 
 * public domain
 */

#define N 8
void main( void ) {
	vec2 v = (gl_FragCoord.xy - tres/2.0) / min(tres.y,tres.x) * 20.0;

	float rsum = 0.0;
	float pi2 = 3.1415926535 * 1.0;
	float C = cos(time*.009 * pi2);
	float S = sin(par_a.x * pi2);
	vec2 shift = vec2( 0.0, 1.0 );
	float zoom = (par_a.y*1.0 + 1.0);
	
	for ( int i = 0; i < N; i++ ){
		float rr = v.x*v.x+v.y*v.y;
		if ( rr > 1.0 ){
			rr = 1.0/rr;
			v.x = v.x * rr;
			v.y = v.y * rr;
		}
		rsum *= .6;
		rsum += rr*(0.1+par_a.z);
		
		v = vec2( C*v.x-S*v.y, S*v.x+C*v.y ) * zoom + shift;
	}
	
	float col = rsum * 22.;


	gl_FragColor = vec4( cos(col*1.0), 1.-cos(col*.1), cos(col*4.0), color.a );

}