precision mediump float;

// Twin Tides
//
// Nested rings gather near a moving center while broad contours bend around a fixed, adjustable anchor. Their boundaries stretch into opposing lobes and narrow waists, like two tides meeting and sliding past one another.
//
// par_a.x - moves the anchor horizontally.
// par_a.y - moves the anchor vertically.
// par_a.z - increases band spacing.

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

//f0:mod 1:
//f1:mod 2:
//f2:relection:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.2, 0.95,  par_a[2]);

#define PI 3.1415926535897932384626
vec2 r = tres;
vec2 mouse = vec2(par_a[0], par_a[1]);

vec3 cc(float x, vec3 a, vec3 b, vec3 c, vec3 d)
{
    return a + b*cos(2.0*PI*cos(c*x + d));
}

void main(void)
{
    vec2 p = 2.0*gl_FragCoord.xy/r - 1.0;
    p.x *= r.x/r.y;
    vec2 m = 2.0*mouse - 1.0;
    m.x *= r.x/r.y;
    p*=2.; m*=2.;
    float l = length(p - vec2(cos(time), -sin(time)));
    float lm = length(p - m) / f2;
    float c = lm/l;
    vec3 color_out = cc(c, vec3(0.5), vec3(0.5), vec3(1.), vec3(0.05, 0.1, 0.11));
    gl_FragColor = vec4(color_out, color.a);
}