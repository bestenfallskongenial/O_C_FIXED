precision mediump float;

// Breathing Flame
//
// Three overlapping radial shapes stretch into narrow tongues, rounded lobes, and rippling fringes. Their contours swell and retreat around the center, like a flame breathing through a folded screen.
//
// par_a.x - increases contour distortion and the reach of its animated movement.
// par_a.y - expands the filled region of the first layer and sharpens its outer transition.
// par_a.z - expands the filled region of the second layer and sharpens its outer transition.
// par_a.w - expands the filled region of the third layer and sharpens its outer transition.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:radial color:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:zoom:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:center converge:
float f3 = mix(0.05, 0.95, par_a[3]);   //f2:center converge:

float saturate(float x) { return clamp(x, 0.0, 1.0); }
float svin(float p) { return sin(p*6.28) * f0; }
float r(vec2 v) { return sqrt(dot(v,v)); }
float a(vec2 v) { return atan(v.y,v.x) / 6.28; }
float a2(vec2 v) { return atan(v.y,v.x) / 6.28 * sin(v.x * 8.0); }

void main( void ) {

vec2 pos = ( gl_FragCoord.xy / tres.xy );// + mouse / 4.0;
vec2 pc = pos * 2.0 - vec2(0.5, 0.5) - .5;
float t = svin((time+svin(time))*0.1) ;
float x = 1.0-smoothstep(f1, 1.0, r(pc) * 1.3 + svin(a2(pc) * 6.0 + t * 3.0) - svin(a2(pc) * 4.0 + 7.0) + t*1.1);
float y = 1.0-smoothstep(f2, 1.0, r(pc) * 1.8 + svin(a2(pc) * 5.7 + t * 7.1) - svin(a2(pc) * 9.0 + 3.0) + t*1.2);
float z = 1.0-smoothstep(f3, 1.0, r(pc) * 1.9 + svin(a2(pc) * 3.0 + t * 4.3) - svin(a2(pc) * 22.0) + t*1.3);
gl_FragColor = vec4(y+z, (x+y) * 0.4, (z+x) * 0.1, color.a );
}
