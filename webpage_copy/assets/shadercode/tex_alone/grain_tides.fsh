precision mediump float;

// Grain Tides
//
// Layers of restless grain fold into one another while the field slowly breathes in scale. Source images can enter the pattern through their brightness, bringing recognizable silhouettes into a shifting wash of texture.
//
// par_a.x - amount of the slow scale variation.
// par_a.y - blends in the first source image.
// par_a.z - blends in the second source image.

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

const float PI = 3.14159265359;

vec2 rotate(vec2 v, float a)
{
    float s = sin(a);
    float c = cos(a);
    return vec2(v.x * c - v.y * s, v.x * s + v.y * c);
}

float noise(vec2 p)
{
    return fract(sin(dot(p, vec2(12.9898, 78.233))) * 43758.5453);
}

float fbm(vec2 p)
{
    float f = 0.0;
    float a = 0.5;
    for (int i = 0; i < 4; i++)
        {
        f += a * noise(p);
        p = rotate(p * 2.0, time * 0.1);
        a *= 0.5;
        }
    return f;
}

vec3 flow(vec2 p)
{
    float n1 = fbm(p + time * vec2(0.1, 0.3));
    float n2 = fbm(p - time * vec2(0.2, 0.1));
    vec2 q = vec2(n1, n2);
    float n3 = fbm(p + 4.0 * q);
    return vec3(n1, n2, n3);
}

void main()
{
    vec2 p = tcoord * 2.0 - 1.0;
    p *= 2.0 + sin(time * 0.1) * f0;
        vec3 col = flow(p);
    col = mix(col, texture2D(tex[0], tcoord).rgb, f1);
    col = mix(col, texture2D(tex[1], tcoord).rgb, f2);
        float intensity = (col.r + col.g + col.b) / 3.0;
    vec3 color1 = vec3(0.5, 0.8, 1.0);
    vec3 color2 = vec3(1.0, 0.2, 0.5);
    vec3 finalColor = mix(color1, color2, intensity);

    gl_FragColor = vec4(finalColor, color.a);
}