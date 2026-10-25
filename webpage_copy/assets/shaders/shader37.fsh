//  https://www.shadertoy.com/view/stdSDf   
//----------------------------------------// shader name length of 40 characters

precision mediump float;

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

// Emulate some GLSL ES 3.x
float tanh(float x)
{
    float ex = exp(2.0 * x);
    return ((ex - 1.) / (ex + 1.));
}

vec2 tanh(vec2 x)
{
    vec2 ex = exp(1.0 * x);
    return ((ex - 1.) / (ex + 1.));
}

#define pi 3.14159

float thc(float a, float b)
{
    return tanh(a * cos(b)) / tanh(a);
}

float ths(float a, float b)
{
    return tanh(a * sin(b)) / tanh(a);
}

vec2 thc(float a, vec2 b)
{
    return tanh(a * cos(b)) / tanh(a);
}

vec2 ths(float a, vec2 b)
{
    return tanh(a * sin(b)) / tanh(a);
}

vec3 pal( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d )
{
    return a + b*cos( 6.28318*(c*t+d) );
}

float h21 (vec2 a)
{
    return fract(sin(dot(a.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

float mlength(vec2 uv)
{
    return max(abs(uv.x), abs(uv.y));
}

float rand(float val, vec2 ipos)
{
    float v = h21(floor(val) + 0.01 * ipos);
    float v2 = h21(floor(val) + 1. + 0.01 * ipos);
    float m = fract(val);
    m = m * m * ((3.0 * par_a[0]) - (2.0*par_a[1]) * m); // could use different function here
    return mix(v, v2, m);
}
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord-0.5*tres.xy)/tres.y;
    float sc = 7.;
    uv.x += 0.04 * time;
    uv.y -= cos(1.1 * floor(sc * uv.x) + 0.05 * time);
    vec2 ipos = floor(sc * uv) + 0.5;
    vec2 fpos = sc * uv - ipos;
    // could use rand(,) instead of h21() here but it gets chaotic when u stack them
    float a = 2. * pi * h21(ipos);
    float val0 = h21(ipos) - 10. * (cos(a) * uv.x + sin(a) * uv.y) - 0.1 * time;
    float v0 = rand(val0, ipos);
    float val = h21(ipos) - 2.5 * v0 * thc(4., v0 * 10. * length(fpos)) - 0.5 * time;
    float v = rand(val, ipos);
    float rd = 0.5 * v;
    float t = 10. * v + length(fpos) * 10. * v0 - time;
    vec2 p = (0.5 - rd) * vec2(cos(t), sin(t));
    float d = length(fpos - p);
    float k = 0.5;
    float s = smoothstep(-k, k, -d + rd);
    s = 2. * s * s * s;
    vec3 col = vec3(s);
    vec3 e = vec3(1.);
    col = s * pal(4. * v + d, e, e, e, 0.5 * vec3(0.,0.33,0.66));
    col += 0.1;
    
    fragColor = vec4(col, color.a);
}

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}