precision mediump float;

// Tidal Lenses
//
// Curved bands gather around an orbiting center and a movable anchor. Their changing distance bends the field into rounded lenses, narrow connections, and tightly nested rings, like two tides pulling a smooth surface between them.
//
// par_a.x - moves the anchor horizontally and increases band spacing.
// par_a.y - moves the anchor vertically while increasing coordinate scale.
// par_a.z - rotates the field through a full turn.

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

    float f2 = mix(0.2, 0.95, par_a[0]);
    float f3 = mix(0.5, 2.0, par_a[1]);
    float f4 = radians(mix(0.0, 360.0, par_a[2]));

#define PI 3.1415926535897932384626

vec2 mouse = vec2(par_a[0], par_a[1]);

vec3 cc(float x, vec3 a, vec3 b, vec3 c, vec3 d)
{
    return a + b*cos(2.0*PI*cos(c*x + d));
}

void main(void)
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);   // UV transformation (zoom)  
    uv *= f3;
    uv = vec2(uv.x * cos(f4) - uv.y * sin(f4), uv.x * sin(f4) + uv.y * cos(f4));    // UV transformation (rotation)
    // Original code starts here
    vec2 p = 2.0*uv;
    p.x *= tres.x/tres.y;
    vec2 m = 2.0*mouse - 1.0;
    m.x *= tres.x/tres.y;
    p*=2.; m*=2.;
    float l = length(p - vec2(cos(time), -sin(time)));
    float lm = length(p - m) / f2;
    float c = lm/l;
    vec3 color_out = cc(c, vec3(0.5), vec3(0.5), vec3(1.), vec3(0.05, 0.1, 0.11));

    gl_FragColor = vec4(color_out, color.a);
}