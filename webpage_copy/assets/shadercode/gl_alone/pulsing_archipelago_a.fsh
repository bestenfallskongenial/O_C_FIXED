precision mediump float;

// Pulsing Archipelago
//
// Rounded fractal lobes form an island surrounded by nested contour regions. A repeating brightness pulse passes through its edges, revealing and concealing small branches like a tide of light moving around a fixed shoreline.
//
// par_a.x - increases coordinate scale, revealing more of the fractal.
// par_a.y - changes the horizontal component of the fractal constant.
// par_a.z - changes the vertical component of the fractal constant.
// par_b.y - blends the tint from the supplied color toward the seed color.

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
   vec2 uv = tcoord;
   vec2 z = uv * 2.0 - 1.0;
   z *= f0 * 3.0;
   
   float n = 0.0;
   for(int i = 0; i < 32; i++) {
       vec2 zn = vec2(
           z.x * z.x - z.y * z.y,
           2.0 * z.x * z.y
       ) + vec2(f1 - 0.5, f2 - 0.5);
       
       if(dot(zn, zn) > 4.0) break;
       z = zn;
       n++;
   }
   
   float t = n + 1.0 - log2(log2(dot(z,z)));
   t = sqrt(t/32.0);
   t = sin(t * 20.0 + time) * 0.5 + 0.5;
   
   vec3 col = mix(
       color.rgb * t,
       vec3(t) * seed.rgb,
       par_b[1]
   );
   
   gl_FragColor = vec4(col, color.a);
}