precision mediump float;

// Orbiting Droplet
//
// Four moving centers gather into a rounded droplet whose outline swells and flexes. Slightly displaced channel edges slip around its perimeter, making the small shape appear to wobble like a suspended bead of liquid.
//
// par_a.x - increases the apparent size of the droplet and its movement.
// par_a.y - changes the spread of its contributing centers.
// par_a.z - strengthens the subtractive field, shrinking or eroding the droplet.

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

float f0 = mix(0.1, 0.95, par_a[0]);    // f0: :
float f1 = mix(0.05, 0.95, par_a[1]);   // f1: :
float f2 = mix(0.05, 0.95, par_a[2]);   // f2: :

#define PI  3.14159265358979323846
#define TAU 6.28318530717958647692

float random(in float val)
{
    return fract(sin(val * 12.9898) * 43758.5453123);
}

float random(in vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

vec2 random2(in float val)
{
    vec2 st = vec2( dot(vec2(val), vec2(127.1, 311.7)), dot(vec2(val), vec2(269.5, 183.3)) );
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

vec2 random2(in vec2 st)
{
    st = vec2( dot(st, vec2(127.1, 311.7)), dot(st, vec2(269.5, 183.3)) );
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

void main()
{
    float d = min(tres.x, tres.y) * 4.0 * f0 + 2.0;
    vec2 st = (gl_FragCoord.xy - (tres.xy / 2.0)) / d;
    vec2 p =  (vec2(f0,f1) - (tres.xy / 2.0)) / d;
    vec3 color_out = vec3(0.0);

    vec3 m_dist = vec3(0.0);
    const int n = 4;    // 6?
    for(int i=0; i<n; i++)
        {
        vec2 pos = random2(vec2(float(i), 0.2)) * 0.2*f1;
        float angle  = mix(0.0,  TAU, random(float(i)));
        float radius = mix(0.01, 0.2, random(float(i)));
        float freq = 1.0;
        if(random(float(i)) > 0.5)
            {
            freq = -1.0;
            }
        vec2 offset = radius*vec2(sin(freq*time + angle), cos(freq*time + angle));
        m_dist.r += 1.0 / distance(st + vec2(0.00, 0.00), pos + offset);
        m_dist.g += 1.0 / distance(st + vec2(0.01, 0.01), pos + offset);
        m_dist.b += 1.0 / distance(st + vec2(0.05, 0.00), pos + offset);
        }
    m_dist.r -= 8.0 *f2 / distance(st + vec2(0.00, 0.00), p);
    m_dist.g -= 8.0 *f2 / distance(st + vec2(0.01, 0.01), p);
    m_dist.b -= 8.0 *f2 / distance(st + vec2(0.05, 0.00), p);
    color_out += smoothstep(0.49, 0.5, m_dist * 0.01);

    gl_FragColor = vec4(color_out, color.a);

//  vec2 uv = ( gl_FragCoord.xx / tres.xy );
}