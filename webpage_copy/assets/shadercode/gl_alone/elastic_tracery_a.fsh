precision mediump float;

// Elastic Tracery
//
// Curved bands and dark seams stretch across a softly shaded surface. Their intersections gather into knots, bow outward into broad lobes, and fold back through the center, like elastic threads drawn across a turning frame.
//
// par_a.x - changes radial wave spacing; higher values spread the central bands farther apart.
// par_a.y - increases the strength of the repeated transformations, changing detail and deformation.
// par_a.z - changes the cosine-driven part of the transformation, reshaping its turning and stretching.

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

float f0 = mix(0.1, 0.95, par_a[0]);    //f0:line count:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:detail:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:mod 1:

#define M_PI 3.141592653589793

void main(void)
{
vec2 position = (gl_FragCoord.xy - 0.5 * tres.xy) / tres.x;

float a = 0.1*time*M_PI/3.0;
float r = length(position) * 2. *f0;
float c = cos(a) * f2;
float s = sin(a);
mat2 rotate = mat2(c,s,-s,c) * f1 + 0.1;
vec2 q = sin(rotate*(position+4.0*position*sin(time+position.yx*2.0)+tan(2.0*position*rotate*rotate))*3.0*(1.5+sin(time*0.5)/r));
q=sqrt(abs(q)-0.1) ;

vec3 colorout = vec3(length(position+q));
colorout.rg += smoothstep(0.2,.0,abs(position.x));
vec2 p2 = (position+q) * rotate;
colorout.gb += smoothstep(0.2,.0,abs(p2.x));
vec2 p3 = p2 * rotate;
colorout.br += smoothstep(0.2,.0,abs(p3.x));
vec2 p4 = p3 * rotate;
colorout.rg += smoothstep(0.2,.0,abs(p4.x));
vec2 p5 = p4 * rotate;
colorout.gb += smoothstep(0.2,.0,abs(p5.x));
vec2 p6 = p5 * rotate;
colorout.br += smoothstep(0.2,.0,abs(p6.x));

colorout.g *= abs(position.x);
colorout.b *= abs(p2.x);
colorout.r *= abs(p3.x);
// colorout = reflect(q.xyy,colorout);
colorout.xy *= rotate;
colorout.yz *= rotate;
colorout = sqrt(abs(reflect(colorout,colorout.zxy)));
gl_FragColor = vec4(colorout, color.a);
}