precision mediump float;

// Inverted Checkerwell
//
// A still checkerboard bends through radial inversion and perspective, gathering into nested cells around a small opening. Broad curved patches surround the center, while their inner edges compress into a tightly folded checkerwell.
//
// par_a.x - changes the strength of the radial inversion.
// par_a.y - shifts the perspective horizon.

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

void main( void )
{
	vec2 pos = ( gl_FragCoord.xy / tres.xy ) * 2.0 - 1.0;
	pos = par_a.x*pos / dot(pos,pos);
	pos *= cos(pos);
    float horizon = par_a.y;
    float fov = 0.85; 
	float scaling = 0.1;
	vec3 p = vec3(pos.x, fov, pos.y - horizon);      
	vec2 s = vec2((p.x)/p.z, p.y/p.z) * scaling;
	//checkboard tex
	float color_out = sign((mod(s.x, 0.1) - 0.05) * (mod(s.y, 0.1) - 0.05));	
	//fading
	color_out *= p.z*p.z*10.0;
	//color_out = fract(color_out);
	gl_FragColor = vec4( vec3(color_out), color.a );
}