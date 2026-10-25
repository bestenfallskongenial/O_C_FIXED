precision mediump float;

// Pleated Waves
//
// Vertical wave bands gather into dense pleats, crossed by rows of openings and sudden narrow seams. Their spacing changes continuously, pulling broad flowing strips into a tighter lattice before releasing them again.
//
// par_a.x - shifts the pattern horizontally and changes one vertical wave frequency.
// par_a.y - shifts the pattern vertically and changes the balance of horizontal and vertical frequencies.
// par_a.z - changes the spacing of two interacting wave layers.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:vert offset:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:colorout pos:

void main( void )
{
vec2 position = ( gl_FragCoord.xy / tres.xy ) + vec2(f0, f1) / 2.0;
float colorout = 0.0;
colorout += cos( position.x * (f2 - f1) * cos( time/ 15.0 ) * 80.0 ) + cos( position.y * tan( time/ 15.0 ) * 10.0 );
colorout += sin( position.y * (f0 * f0) * tan( time/ 10.0 ) * 40.0 ) + tan( position.x * sin( time/ 25.0 ) * 40.0 );
colorout += sin( position.y * (f1 + f2) * sin( time/ 55.0 ) * 10.0 ) + sin( position.x * cos( time/ 35.0 ) * 80.0 );
colorout *= tan( time/ 10.0 ) * 0.5;

gl_FragColor = vec4( vec3( colorout, colorout * 0.5, tan( colorout + time/ 5.0 ) * 0.75 ), color.a );
}