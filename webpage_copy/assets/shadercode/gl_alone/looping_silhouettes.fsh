precision mediump float;

// Looping Silhouettes
//
// Bold curved silhouettes twist around nested circular openings. Repeated boundaries gather into fine rings, then stretch into broad connected shapes, like a cut-paper surface folding through its own loops.
//
// par_a.x - changes the strength of the radial inversion, reshaping the central openings.
// par_a.y - increases the distance-driven twist, tightening the surrounding loops.

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

#define tau 6.28318530718

#define rot( a ) mat2( cos( a ), -sin( a ), sin( a), cos( a ) )

vec2 p, m = mouse*tres;
vec3 c = vec3( 0. ); vec3 c0 = vec3( 0. );
float circle( vec2 p, float r ) { return step( length( p ), r ); }
vec3 hueShift( float h ) { vec4 k = vec4( 1., 2./3., 1./3., 3. ); vec3 p = sin( fract( vec3( h ) + k.xyz )*6. - k.www ); return mix( vec3( 1. ), clamp( p - vec3( 1. ), 0., 1. ), 1. ); }

void mainImage( vec2 p )
	{ p *= rot( 2.*time );
	p *= ( .25*( mouse.x + .1 ) )/dot( p, p ); p -= dot( 1.8-p, p );
	float lp = length( p*27.*( mouse.y + .2) );
	p *= rot( lp + 1.5*time );
	float theta = atan( .01/p.y, p.x*p.y + p.y )*sin( p.y );
	c += step( sin( theta*7. ), 0. ) - step( cos( theta*1. ), 1.);
	c += circle( p, 1. ); }

void main( void )
	{ m = 1.*( m.xy - tres/2. )/tres.y;
	for ( float kk = 0.; kk < 1.0; kk++ ) for ( float kkk = 0.; kkk < 1.0; kkk++ ) // nested 1.0 loop
    		{ vec2 k = ( vec2( kk, kkk ) - .5 )/vec2( 1.0 ); c = vec3( 0. );
                p = 1.*( gl_FragCoord.xy - k - tres/2. )/tres.y;    
		mainImage( p );	c0 += c; }
	c0 /= 1.0*1.0;
	gl_FragColor = vec4( c0, color.a ); }////ändrom3da