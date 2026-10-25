precision mediump float;

// Oscillating Drapes
//
// Crossing waves gather into vertical curtains, dotted rows, and sudden narrow openings. Their spacing stretches and contracts as the layers pass through one another, turning broad strips into a finer lattice before pulling it apart again.
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

vec2 mouse = par_a.xy;

float f0 = mix(0.2, 0.95, par_a[0]);    //f0:checker size:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:wave size:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:color rotate:

void main( void ) {

vec2 position = ( gl_FragCoord.xy / tres.xy ) + vec2(f0,f1) / 1.0;

float color_out = 0.0;
color_out += sin( position.x * cos( time / 15.0 ) * 80.0 ) + cos( position.y * cos( time / 15.0 ) * 10.0 );
color_out += sin( position.y * sin( time / 10.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );
color_out += sin( position.x * cos( time / 5.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );
color_out += tan( position.x * sin( time / 5.0 ) * 10.0 ) + cos( position.y * sin( time / 35.0 ) * 80.0 );
color_out *= sin( time / 10.0 ) * f2;

gl_FragColor = vec4( vec3( color_out, color_out * 0.5, sin( color_out + time / 1.0 ) * 9.75 ), color.a );

}