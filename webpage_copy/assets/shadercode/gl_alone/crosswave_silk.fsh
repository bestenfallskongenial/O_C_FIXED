precision mediump float;

// Crosswave Silk
//
// Horizontal and vertical waves bend a smooth field into broad, softly moving patches. Their intersections stretch and relax like silk pulled from two directions, while a diagonal wave joins the folds into a continuous flowing surface.
//
// par_a.x - increases horizontal distortion frequency.
// par_a.y - increases vertical distortion frequency.
// par_a.z - increases distortion strength.
// par_b.x - blends toward the supplied solid color.
// par_b.y - blends toward the seed color.

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
   vec2 uv = tcoord * 2.0 - 1.0;
   
   float wave1 = sin(uv.x * 10.0 * f0 + time);
   float wave2 = cos(uv.y * 10.0 * f1 + time);
   
   vec2 distort = vec2(
       wave1 * f2,
       wave2 * f2
   );
   
   uv += distort * 0.2;
   
   vec3 col = vec3(
       sin(uv.x * 3.14159),
       cos(uv.y * 3.14159),
       sin((uv.x + uv.y) * 3.14159)
   );
   
   col = mix(col, color.rgb, par_b[0]);
   col = mix(col, seed.rgb, par_b[1]);
   
   gl_FragColor = vec4(col, color.a);
}