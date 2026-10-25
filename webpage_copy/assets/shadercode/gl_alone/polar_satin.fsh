precision mediump float;

// Polar Satin
//
// Angular and radial gradients bend through small crossing waves. Their smooth regions drift around a central seam, forming broad folds that shift like satin stretched over a gently turning surface.
//
// par_a.x - increases the drift of the angular gradient.
// par_a.y - increases the radial gradient’s scale.
// par_a.z - increases the frequency of the crossing distortions.
// par_b.x - blends the generated field toward the supplied solid color.

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
   vec2 uv = tcoord - 0.5;
   float angle = atan(uv.y, uv.x);
   float radius = length(uv);
   
   vec2 warped = vec2(
       angle/6.28318 + time * f0,
       radius * f1
   );
   
   vec2 twisted = warped + vec2(
       sin(warped.y * 6.28318 * f2),
       cos(warped.x * 6.28318 * f2)
   ) * 0.1;
   
   vec3 col = vec3(
       twisted.x,
       twisted.y,
       (twisted.x + twisted.y) * 0.5
   );
   
   col = mix(col, col.brg, seed.x);
   col = mix(col, color.rgb, par_b[0]);
   
   gl_FragColor = vec4(col, color.a);
}