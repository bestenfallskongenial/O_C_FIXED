precision mediump float;

// Spiral Imprint
//
// The red channel of an external image repeats through a spiral tunnel mapping. Its details pack tightly toward the center while the sampling continuously advances, turning the picture into concentric impressions threaded by a slight angular twist.
//
// par_a.x - increases radial repetition, packing the source texture more tightly toward the center.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:count:
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);


void main()
{
   vec2 p = (tcoord - 0.5) * (tres / tres.x) * 0.1;  // 0.1 for 10x zoom
   float c = 0.0;
   
   float l = 1.0 / length(p) * f0;
   p = vec2(9.0 * atan(p.y, p.x) / (4.0 * asin(1.0)), l);
   p.y += p.x / 3.0 + 2.0 * time;
   p.y /= 3.0;
   c = mod(floor(p.x) + floor(p.y), 1.0);
   c = mix(c, texture2D(tex[0], fract(vec2(p.y, -p.x))).x, 0.8);
   c == pow(0.6, l / log(tres.x));
   
   gl_FragColor = vec4(c, c, c, color.a );
}