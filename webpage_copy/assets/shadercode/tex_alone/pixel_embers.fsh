precision mediump float;

// Pixel Embers
//
// Small square cells appear and disappear as drifting noise crosses a moving threshold. Each cell fades toward its corners, creating a field of softly glowing pixels that flicker in loose clusters. An external image can gradually replace the generated pattern.
//
// par_a.x - increases the number of columns and the speed of threshold oscillation.
// par_a.y - increases the number of rows.
// par_a.z - blends from the generated cells toward the source image.

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

float noise(vec2 st)
{
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    vec2 st = tcoord * vec2(100.0 * par_a[0], 100.0 * par_a[1]);
    vec2 ipos = floor(st);
    vec2 fpos = fract(st);
        float n = noise(ipos + time * 0.1);
    float threshold = 0.5 + 0.3 * sin(time * par_a[0]);
        vec3 color_out = vec3(0.0);
    if (n > threshold)
{
        color_out = vec3(1.0, 0.5, 0.2);
    }
else if (n > threshold - 0.1)
{
        color_out = vec3(0.2, 0.5, 1.0);
    }
        color_out *= 1.0 - length(fpos - 0.5);
    color_out = mix(color_out, texture2D(tex[0], tcoord).rgb, par_a[2]);
        gl_FragColor = vec4(color_out, 1.0);
}