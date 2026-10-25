precision mediump float;

// Rolling Thresholds
//
// Rounded forms emerge from repeating gradients, their smooth shading interrupted by sharp moving boundaries. The surface continually rolls from light to dark, like cloth passing over a hidden arrangement of curved blocks.
//
// par_a.x - increases horizontal repetition, making the forms narrower.
// par_a.y - increases vertical repetition, making the forms shorter.
// par_a.z - shifts the gradient phase and the position of its sharp boundaries.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:horz stretch:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert stretch:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:gradient pos:

void main( void )
{ 
gl_FragColor = vec4( mod(time+ f2 + cos(gl_FragCoord.x / 50. * f0) * sin(gl_FragCoord.y / 50.0 * f1), color.a)   );  // was ,1.)
}
