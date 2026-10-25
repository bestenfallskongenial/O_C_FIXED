precision mediump float;

// Three Quiet Portals
//
// Three rounded square outlines gather around a small glowing center. Moving slowly, they drift apart and overlap again, opening a succession of shallow, nested doorways.
//
// par_a.x - spread of the portal arrangement.
// par_a.y - size of the outlines.
// par_a.z - phase separation of their orbital movement.

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

float f0 = mix(0.05, 0.95, par_a.x);   //f0:spread:
float f1 = mix(0.05, 0.95, par_a.y);   //f1:zoom freq:
float f2 = mix(0.05, 0.95, par_a.z);   //f2:offset:
 
void main()
{
   float t = time * 0.001;  // Slowed down the time progression
   vec2 r = tres;       vec2 p = tcoord;
   p = p * 2.0 - 1.0;
   p.x *= tres.x / tres.y;       vec3 destColor = vec3(0.0);
   for (float i = 1.0; i < 4.0; i++)
{
      float j = i;
      j *= f2;
      vec2 q = p + vec2(cos(t * j), sin(t * j)) * f0;
      float l = 0.02 / abs(length(q*q) - f1 * i * abs(tan(t)));
      destColor += vec3(abs(tan(t))*l, abs(sin(t))*l, abs(cos(t))*l*l);
   }
      gl_FragColor = vec4(destColor, color.a);

   // Example of using textures if needed:
   // vec4 texColor1 = texture2D(tex[0], tcoord);
   // vec4 texColor2 = texture2D(tex[1], tcoord);
   // gl_FragColor *= texColor1;
}