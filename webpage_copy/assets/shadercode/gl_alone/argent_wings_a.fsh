precision mediump float;

// Argent Wings
//
// Fine white lines open into mirrored wings, nested diamonds, and crossing spokes. Their geometry breathes between a small central knot and broad angular fans, suspended inside a soft silver glow.
//
// par_a.x - changes the contraction and expansion of the line geometry, with a slight phase offset.
// par_a.y - changes the starting proportions of the mirrored shapes.
// par_a.z - darkens the surrounding glow and sharpens the contrast around the lines.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:shape offset:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:shape mode:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:bg shade:

#define TAU 6.28318530718

float segment(vec2 p, vec2 a, vec2 b)
{
    vec2 ab = b - a;
    vec2 ap = p - a;
    float k = clamp(dot(ap, ab)/dot(ab, ab), 0.0, 1.0);
    return smoothstep(0.0, 5.0/tres.y, length(ap - k*ab) - 0.001);
}

float shape(float u, vec2 p, float angle)
{
    float d = 100.0;
    vec2 a = vec2(1.0, f1), b;
    vec2 rot = vec2(cos(angle), sin(angle)*u);

    for (int i = 0; i < 3; ++i)
        {
        b = a;
        for (int j = 0; j < 3; ++j)
            {
            b = vec2(b.x*rot.x - b.y*rot.y, b.x*rot.y + b.y*rot.x);
            d = min(d, segment(p, a, b));
            }
        a = vec2(a.x*rot.x - a.y*rot.y, a.x*rot.y + a.y*rot.x);
        }
    return d;
}

void main(void)
{
    float localTime = time * 10.0;
    vec2 uv = gl_FragCoord.xy / tres.xy;
    vec2 cc = (-tres.xy + 2.0*gl_FragCoord.xy) / tres.y;
    float numPhases = 40.5;
    float u = 0.05 + (f0 * 2.2);
    float t = f0/tres.x * numPhases;
    float SpeedReal = 2.0;

    float col = shape(u, abs(cc), t + cos(0.01 * (localTime * SpeedReal)) * TAU);
    col *= 0.5 + 1.5 * pow(uv.x * uv.y * (1.0 - uv.x) * (1.0 - uv.y), 0.3);

    vec3 finalColor = vec3(pow(1.0 - col, 2.15 * f2));

    gl_FragColor = vec4(finalColor, color.a);
}