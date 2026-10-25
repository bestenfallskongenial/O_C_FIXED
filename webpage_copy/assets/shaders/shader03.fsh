// Transposed Complex Pattern Shader                    
//--------------------------// shader name length of 26 characters

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

//f0:brightness:
//f1:rotate:
//f2:grid density:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

const float pi = 3.1415926535;

vec2 clog(vec2 v)
{
    return vec2(0.5 * log(v.x * v.x + v.y * v.y), atan(-v.y, v.x));
}

vec2 cdiv(vec2 a, vec2 b)
{
    return vec2(a.x * b.x + a.y * b.y, a.y * b.x - a.x * b.y) / (b.x * b.x + b.y * b.y);
}

vec4 checkerboard(vec2 pos)
{
    float a = (fract(pos.x) - 0.5) * (fract(pos.y) - 0.5);
    return (a > 0.0) ? vec4(0.7, 0.72, 0.7, 1.0) : vec4(0.3, 0.33, 0.3, 1.0);
}

vec4 grid(vec2 pos, float thickness)
{
    vec2 a = abs(fract(pos) - 0.5);
    return (a.x > thickness && a.y > thickness) ? vec4(0.7, 0.72, 0.7, 1.0) : vec4(0.3, 0.33, 0.3, 1.0);
}

vec4 wave(vec2 pos)
{
    float a = fract(pos.x + 0.5 * sin(pos.y * 1.0 * pi));
    return (a > 0.5) ? vec4(0.7, 0.72, 0.7, 1.0) : vec4(0.3, 0.33, 0.3, 1.0);
}

vec4 rainbow(vec2 pos)
{
    return vec4(sin(pos.x * 2.0 * pi) + 1.0, 
                sin(pos.x * 2.0 * pi + pi * 1.0 / 3.0) + 1.0, 
                sin(pos.x * 2.0 * pi + pi * 2.0 / 3.0) + 1.0, 
                1.0);
}

void main()
{
    vec2 position = 2.0 * (gl_FragCoord.xy / tres.xy) - 1.0;
    const float p1 = 1.0;
    const float p2 = 1.0;
    float u_corner = 2.0 * pi * p2;
    float v_corner = log(256.0) * p1;
    float diag = sqrt(u_corner * u_corner + v_corner * v_corner);
    float sin_a = v_corner / diag;
    float cos_a = u_corner / diag;
    float scale = diag / 2.0 / pi;
    float offset = 0.4;
    
    vec2 p = clog(position + vec2(offset, 0.0)) - clog(position + vec2(-offset, 0.0));
    vec2 rotated = vec2(p.x * cos_a - p.y * sin_a, p.x * sin_a + p.y * cos_a) * f2;
    vec2 scaled = rotated * scale / vec2(log(256.0), 2.0 * pi);
    vec2 translated = scaled - vec2(time, 0.0) * sin(f1 * 0.005);
    
    vec4 color_out = wave(translated * 8.0 + 0.3);
    color_out -= checkerboard(translated * 4.0);
    color_out += rainbow(translated) * f0;
    
    gl_FragColor = vec4(color_out.rgb, color.a );
}