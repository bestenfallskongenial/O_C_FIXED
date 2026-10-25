precision mediump float;

// Breathing Hollow
//
// Three offset radial fields surround a soft central hollow. Their wandering centers pull the surrounding gradients into an uneven, breathing enclosure, while mid-frequency audio can brighten the field around its dark interior.
//
// par_a.x - increases horizontal movement speed of the radial centers.
// par_a.y - increases vertical movement speed of the radial centers.
// par_a.z - contracts the central hollow and brings surrounding gradients inward.
// par_b.x - increases sensitivity to mid-frequency audio.
// par_b.y - increases overall brightness.

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
   vec3 finalColor = vec3(0.0);
   
   // Create nebula base
   float nebula = 0.0;
   for(float i = 1.0; i < 4.0; i++) {
       vec2 offset = vec2(sin(time * f0 * i), cos(time * f1 * i)) * 0.3;
       nebula += 1.0/i * smoothstep(0.2, 0.8, 
           length(uv + offset) * f2);
   }

   // Add swirling motion
   float swirl = atan(uv.y, uv.x) * f1 + time * 0.5;
   
   // Combine with audio reactivity
   nebula *= 1.0 + audio.y * par_b[0];
   
   // Color mapping
   finalColor = vec3(
       nebula * color.r + seed.x,
       nebula * color.g + seed.y,
       nebula * color.b + seed.z
   ) * par_b[1];
   
   gl_FragColor = vec4(finalColor, color.a);
}