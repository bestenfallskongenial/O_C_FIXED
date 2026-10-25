precision mediump float;

// Cellular Procession
//
// Small discs shift within a regular grid while a traveling radial pulse reveals and conceals them. Their clipped intersections form crescents and fragments, sending loose clusters across the arrangement like a procession passing behind moving shutters.
//
// par_a.x - increases grid density, making the cells smaller.
// par_a.y - increases phase differences in the radial reveal pattern.
// par_a.z - changes the phase relationships of the cell offsets across rows and columns.
// par_b.x - blends the pattern toward the solid seed color.

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


float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

void main() {
   vec2 uv = tcoord * f0 * 20.0;
   vec2 id = floor(uv);
   vec2 gv = fract(uv) - 0.5;
   
   float value = step(
       length(gv),
       sin(length(id) * f1 + time) * 0.5 + 0.5
   );
   
   vec2 offset = vec2(
       sin(id.x * f2 + time),
       cos(id.y * f2 + time)
   ) * 0.25;
   
   value *= step(length(gv + offset), 0.3);
   
   vec3 col = mix(
       color.rgb * value,
       seed.rgb,
       par_b[0]
   );
   
   gl_FragColor = vec4(col, color.a);
}