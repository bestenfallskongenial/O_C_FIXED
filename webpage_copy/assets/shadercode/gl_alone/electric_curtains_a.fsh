precision mediump float;

// Electric Curtains
//
// Crossing waves form shifting vertical bands, rows of soft spots, and sudden narrow breaks. Their spacing continually stretches and contracts, turning a striped curtain into a loose lattice before pulling it apart again.
//
// par_a.x - shifts the pattern horizontally.
// par_a.y - shifts the pattern vertically.
// par_a.z - increases wave-signal strength, changing contrast and banding.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:horz offset:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert offset:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:colorout mod:

void main( void )
{
vec2 position = ( gl_FragCoord.xy / tres.xy ) + vec2(f0,f1) / 1.0;

float colorout = 0.0;
colorout += sin( position.x * cos( time/ 15.0 ) * 80.0 ) + cos( position.y * cos( time/ 15.0 ) * 10.0 );
colorout += sin( position.y * sin( time/ 10.0 ) * 40.0 ) + cos( position.x * sin( time/ 25.0 ) * 40.0 );
colorout += sin( position.x * cos( time/ 5.0 ) * 10.0 ) + sin( position.y * sin( time/ 35.0 ) * 80.0 );
colorout += tan( position.x * sin( time/ 5.0 ) * 10.0 ) + cos( position.y * sin( time/ 35.0 ) * 80.0 );
colorout *= sin( time/ 10.0 ) * f2;

gl_FragColor = vec4( vec3( colorout, colorout * 0.5, sin( colorout + time/ 1.0 ) * 9.75 ), color.a );
}