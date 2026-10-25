precision mediump float;

// Interference Lanterns
//
// Crossing wave fields gather into pointed columns, rounded cells, and irregular openings. Their boundaries slide through one another, continually turning an orderly arrangement into a looser collection of glowing, lantern-like forms.
//
// par_a.x - changes vertical frequency in the first wave layer.
// par_a.y - changes horizontal frequency in the second wave layer.
// par_a.z - changes vertical frequency in the third wave layer.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:mod 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:mod 2:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:mod 3:

void main( void )
{
vec2 position = gl_FragCoord.xy / tres.xy;

float color_out = 0.0;
color_out = sin( position.x * cos(  time / 166.0 ) * 20.0 ) + cos( position.y * cos(  time / 165.0 ) * 10.0 * f0 );
color_out *= sin( position.y * sin(  time / 10.0 ) * 40.0 ) + cos( position.x * sin(  time / 225.0 ) * 100.0 * f1);
color_out *= sin( position.x * sin(  time / 5.0 ) * 10.0 ) + sin( position.y * sin(  time / 35.0 ) * 80.0 * f2);
color_out -= sin(  time / 100.0 ) * 0.5;
gl_FragColor = vec4( vec3( color_out * sin(  time * 2.0), color_out * 0.5 / time/0.001, sin( color_out +  time / 100.0 ) * 0.45 ), color.a );
}