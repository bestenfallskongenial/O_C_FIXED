precision mediump float;

// Orbiting Ripples
//
// An orbiting center draws nested contours through a field shaped by a second anchor. Broad rounded regions swell between them, then narrow into sweeping seams as the moving center passes around its path.
//
// par_a.x - moves the anchor horizontally.
// par_a.y - moves the anchor vertically.
// par_a.z - increases band spacing.
// par_a.w - increases coordinate scale, revealing a wider portion of the field.
// par_b.x - rotates the field through a full turn.

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

    vec2 mouse = vec2(par_a[0], par_a[1]);

    float f2 = mix(0.2, 0.95, par_a[2]);
    float f3 = mix(0.5, 2.0, par_a[3]);
    float f4 = radians(mix(0.0, 360.0, par_b[0]));

#define PI 3.1415926535897932384626

vec3 cc(float x, vec3 a, vec3 b, vec3 c, vec3 d)
{
    return a + b*cos(2.0*PI*cos(c*x + d));
}

void main(void)
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
    
    uv *= f3;   // UV transformation (zoom)
    
    uv = vec2(uv.x * cos(f4) - uv.y * sin(f4), uv.x * sin(f4) + uv.y * cos(f4)); // UV transformation (rotation)

    vec2 p = 2.0*uv;    // Original code starts here
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