precision mediump float;

// Petalled Rings
//
// Repeated ring motifs wrap around a radial field, changing between rounded cells, diamond-like openings, and petalled bands. Their contours pulse together, forming a layered rosette that continually opens and tightens around its center.
//
// par_a.x - moves the angular mapping anchor horizontally.
// par_a.y - moves the angular mapping anchor vertically.
// par_a.z - increases radial repetition.

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

    float f0 = mix(0.05, 0.95, par_a[0]);   //f0:center x:
    float f1 = mix(0.05, 0.95, par_a[1]);   //f1:center y:
    float f2 = mix(0.05, 0.95, par_a[2]);   //f2:zoom:

vec3 Hue(float hue)
{
    vec3 rgb = fract(hue + vec3(0.0, 2.0/3.0, 1.0/3.0));
    rgb = abs(rgb * 2.0 - 1.0);
    return clamp(rgb * 3.0 - 1.0, 0.0, 1.0);
}

vec3 HSV2RGB(vec3 hsv)
{
    return ((Hue(hsv.x) - 1.0) * hsv.y + 1.0) * hsv.z;
}

float lengthN(vec2 v, float n)
{
    vec2 tmp = pow(abs(v), vec2(n));
    return pow(tmp.x + tmp.y, 1.0/n);
}

float rings(vec2 p)
{
    vec2 p2 = mod(p * 8.0, 4.0) - 2.0;
    float r2 = 1.0 - abs(sin(p.x * 0.02 + time * 0.4));
    float r3 = 4.0 + (sin(time * 0.4) * 2.0);
    float p3 = sin(lengthN(p2, 1.0 + r2 * 10.0) * r3);
    return pow(p3, 0.25);
}

vec2 trans(vec2 p)
{
    vec2 p1;
    p1.x = f0 * 2.0 - 1.0;
    p1.y = f1 * 2.0 - 1.0;
    float theta = atan(p.y + p1.y, p.x + p1.x);
    float r = length(p * f2);
    return vec2(theta, r);
}

void main(void)
{
    vec2 pos1 = (gl_FragCoord.xy / tres.xy) / 4.0;
    vec2 pos2 = (gl_FragCoord.xy * 2.0 - tres) / tres.y;

    float h = gl_FragCoord.x / tres.x;
    float s = 0.7;
    float v = rings(trans(pos2)) * 0.5 + 0.25;
    h = sin(h * 0.1 + time * 0.5) * 0.5;

    vec3 color_out = HSV2RGB(vec3(h, s, v));

    gl_FragColor = vec4(color_out, color.a);
}