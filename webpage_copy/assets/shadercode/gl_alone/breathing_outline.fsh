precision mediump float;

// Breathing Outline
//
// A rounded outline slowly reshapes into uneven lobes, narrow waists, and stretched loops. Its boundary flexes around the center like a soft hoop being pulled by several moving points.
//
// par_a.x - changes angular frequency of the horizontal deformation.
// par_a.y - changes angular frequency of the vertical deformation.
// par_a.z - increases deformation strength.
// par_b.x - blends from the supplied solid color toward the procedural contour.

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
   vec2 p = tcoord * 2.0 - 1.0;
   vec2 q = p;
   
   float r = length(p);
   float a = atan(p.y, p.x);
   
   p += vec2(
       cos(a * f0 * 3.0 + time),
       sin(a * f1 * 3.0 + time)
   ) * (r * f2);
   
   float shape = length(p) - 0.5;
   shape = smoothstep(0.0, 0.1, abs(shape));
   
   vec3 col = mix(
       color.rgb,
       vec3(shape),
       par_b[0]
   );
   
   col = mix(col, col.grb, seed.x);
   
   gl_FragColor = vec4(col, color.a);
}