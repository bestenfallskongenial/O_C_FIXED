precision mediump float;

// Spherical Ribbons
//
// Nested circular bands fold through paired spherical transformations. The surface expands, contracts, and passes through itself, opening into loops and broad ribbon-like regions as the repeating cycle advances.
//
// par_a.x - changes the orientation and scale of the paired transformations together with par_a.y.
// par_a.y - changes the orientation and scale of the paired transformations together with par_a.x.

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

const float PI = 3.1415926;
const float TAU = PI * 2.0;

#define slow 1.0/32.0
#define time ((mod(time*slow,2.0))-1.0)

vec3 c2sA(vec2 p)
{
	vec2 p2 = p * p;
	float md = (1.0 + p2.x + p2.y);
	return vec3( p.xy / md, (-1.0 + p2.x + p2.y) / (2.0 * md) );
}

vec2 s2cA(vec3 p)
{
	return p.xy / (0.5 - p.z);
}

vec3 c2sB(vec2 p)
{
	vec2 p2 = p * p;
	float omd = 1.0 / (1.0 + p2.x + p2.y);
	return vec3( 2.0 * p.xy, (-1.0 + p2.x + p2.y) ) * omd;
	
}

vec2 s2cB(vec3 p)
{
	return p.xy / (1.0 - p.z);
}


void main( void )
{
	float t = time;
	vec2 p = ((2.0*gl_FragCoord.xy - tres.xy)/min(tres.x, tres.y));
	vec2 sp = (tcoord - 0.5) * 5.0; // Center and scale the UV coordinates
	vec2 v = mix( sp - p/2.0, sp, sin(t*TAU) );
	float dp = dot(v,v);
	v=mix(v*dp,v/dp,cos(dp+t*TAU*TAU)*0.5+0.5);
	vec3 s = (c2sA( v ) + c2sB( v ));
	vec2 m = (mouse * 2.0 - 1.0) * PI;
	vec2 sc = m / dot(m,m);
	s.xy *= mat2( sc.x, -sc.y, sc.y, sc.x );
	s.yz *= mat2( sc.x, -sc.y, sc.y, sc.x );
	vec4 o = vec4( fract(s), color.a);

	gl_FragColor = o;

}