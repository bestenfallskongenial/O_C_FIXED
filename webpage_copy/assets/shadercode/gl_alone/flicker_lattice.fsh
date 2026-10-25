precision mediump float;

// Flicker Lattice
//
// A fine grid breaks into small blocks and narrow intersecting lines. Individual regions pulse at different rates, creating a shifting patchwork of bright cells and dark gaps, like windows flickering across a restless facade.
//
// par_a.x - increases the overall number of cells.
// par_a.y - changes the tangent-grid frequency, reshaping the intersecting lines and blocks.
// par_a.z - reduces overall brightness.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers  uniform int tex_l;     
     //f0:zoom:
//f1:grid zoom:
//f2:density:

float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

  
float rand (in vec2 seed)
{
return fract (sin (dot (seed, vec2 (12.9898, 18.233))) * 137.5453);
}
void main(void)
{

vec2 frag = (2.0 * gl_FragCoord.xy - tres.xy) / tres.y;
//frag *= 1.0 - 0.2 * cos (frag.yx) * sin (3.14159 * 0.1);// * tex (iChannel0, vec2 (0.0)).x);
frag *= 5.0 +  (f0 * 10.);
float random = rand (floor (frag));
vec2 black = smoothstep (2.0, 0.9, tan (frag * 3.14159 * (f1 + .5 ) ) );
vec3 color_out = vec3(0.75/f2); color_out *= black.x * black.y * smoothstep (2.0, 0.9,length(fract(frag) - 0.5));
color_out *= 0.9 + 0.9 * cos (random + random * ( time * 9.0) +  time + 3.14159 * 1.0); // * tex (iChannel0, vec2 (0.7)).x);
gl_FragColor = vec4 (color_out * vec3(1.0,1.0,1.0), color.a );

//   vec2 uv = ( gl_FragCoord.xx / tres.xy );
   }//endof main


