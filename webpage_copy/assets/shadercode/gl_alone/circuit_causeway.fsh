precision mediump float;

// Circuit Causeway
//
// Zigzag tracks and narrow luminous lines recede toward a shared horizon. Traveling pulses pass along the traces while fine ripples cross their surfaces, suggesting a circuit stretched into an advancing causeway.
//
// par_a.x - changes perspective depth and spacing of the circuit traces.
// par_a.y - increases the strength and spread of the additional glow.
// par_a.z - changes depthwise repetition of the finer circuit tracks.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:stretch:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:beams:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:zigs:

const float PI  = 3.141592653589793;
const float PI2 = PI * 2.0;
#define saturate(x) clamp(x,0.0,1.0)

float tri(float x)
{
    return abs(2.0 * fract(x * 0.5 - 0.25) - 1.0) * 2.0 - 1.0;
}

float sqr(float x)
{
    return -2.0 * (step(0.5, fract(x * 0.5)) - 0.5);
}

float circuit(float x)
{
    return clamp(tri(x * 4.0) * 0.25, (sqr(x) - 1.0) * 0.5, (sqr(x + 0.5) + 1.0) * 0.5);
}

float circuit2(float x)
{
    return clamp(tri(x * 0.5) * 0.5, 0.0, 0.25);
}

float flow(float x, float d)
{
    return (1.0 + sin(x + d * time * 5.0 + 3.0)) * 0.3 + 0.3;
}

vec2 pp(vec2 p)
{
    float r = f0 / p.y;
    return vec2(p.x * r, r);
}

void main(void)
{
    float t = floor(time * 30.0) / 15.0;
    vec2 p = (gl_FragCoord.xy * 2.0 - tres) / min(tres.x, tres.y);
    float l = 0.0, d = sign(p.y);
    vec2 r = p;
    vec2 q = p / f2;
    q = pp(q);
    float n = floor(q.x / 0.5);
    q.x = mod(q.x, 0.5) - 0.25;
    p = pp(p);
    l += step(abs(q.x + circuit(q.y + sign(p.y) * t + n)), 0.005);
    l += pow(0.001 / abs(p.x + circuit(p.y + d * t)), flow(p.y, d));
    l += pow(0.001 / abs(p.x + 0.75 + circuit2(p.y + d * t)), (1.0 + sin(p.y + d * time * 5.0)) * 0.3 + 0.3);
    l += pow(0.001 / abs(p.x - 0.75 + circuit2(p.y + d * t)), (1.0 + sin(p.y + d * time * 5.0)) * 0.3 + 0.3);
    l = saturate(l);
    l *= abs(r.y * 2.0);
    l *= abs(sin(r.y * 200.0 + sign(p.y) * time * 5.0)) * 0.5 + 0.5;

    if (p.y < 240.0)
        {
        l += pow(0.15 * f1 / abs(p.x + circuit(p.x + d * time)), (1.0 + sin(p.x + d * time * 5.0 + 3.0)) * 0.3 + 0.3);
        }

    vec3 color_out = vec3(l) - vec3(0.3, 0.1, 0.0);

    gl_FragColor = vec4(color_out, color.a);
}