precision mediump float;

// Reactive Veil
//
// An external image is transformed through interactions between its red and green channels and their neighboring samples. The resulting differences become a slowly cycling tonal veil, which can blend back into the original image or a second source.
//
// par_a.x - increases red-channel decay while blending the source textures toward the second image.
// par_a.y - increases red-channel decay.
// par_a.z - increases red-channel diffusion and green-channel decay while blending toward the source images.
// par_a.w - increases green-channel diffusion.

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
 
const float PI = 3.14159265359;

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

vec2 reaction_diffusion(vec2 uv)
{
    float step = 1.0 / 256.0;
    vec2 sum = vec2(0.0);
    for(int i = -1; i <= 1; i++)
{
        for(int j = -1; j <= 1; j++)
{
            sum += texture2D(tex[0], uv + vec2(float(i), float(j)) * step).rg;
        }
    }
    vec2 center = texture2D(tex[0], uv).rg;
    float a = center.r;
    float b = center.g;
    float da = par_a[2] * (sum.r - 9.0 * a) + a * a * b - (par_a[0] + par_a[1]) * a;
    float db = par_a[3] * (sum.g - 9.0 * b) + a * a * b - par_a[2] * b;
    return clamp(center + vec2(da, db) * 0.1, 0.0, 1.0);
}

void main()
{
    vec2 uv = tcoord;
    vec2 rd = reaction_diffusion(uv);
        float v = rd.r - rd.g;
    vec3 col = vec3(v);
        float t = time * 0.1;
    vec3 color1 = vec3(sin(t) * 0.5 + 0.5, sin(t + 2.0) * 0.5 + 0.5, sin(t + 4.0) * 0.5 + 0.5);
    vec3 color2 = vec3(sin(t + PI) * 0.5 + 0.5, sin(t + PI + 2.0) * 0.5 + 0.5, sin(t + PI + 4.0) * 0.5 + 0.5);
        col = mix(color1, color2, v);
        vec3 texCol = mix(texture2D(tex[0], uv).rgb, texture2D(tex[1], uv).rgb, par_a[0]);
    col = mix(col, texCol, par_a[2]);
        gl_FragColor = vec4(col, 1.0);
}