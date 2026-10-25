// BW tunnel                                  
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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:center x:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:center y:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:count:

#define pi 3.1415927410125

float rand(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec2 pos = gl_FragCoord.xy / tres - vec2(f0, f1);
    pos.x *= (tres.x / tres.y);
    float u = length(pos);
    float v = atan(pos.y, pos.x) / f2;
    float t = time / 0.8 + abs(sin(cos(time)) + 1.0 * 2.9) / u;
    float intensity = abs(sin(t * 4.0 + v) + sin(v * 1.0)) * 0.325 * u * 0.325;
    float one = -sin(v * 4.0 + v * 2.0 + time);
    float two = sin(u * 8.0 + v - time);
    float three = cos(u + v * 3.0 + time);
    float t1 = abs(sin(time + (2.0 * pi / 3.0 * 0.0)));
    float t2 = abs(sin(time + (2.0 * pi / 3.0 * 1.0)));
    float t3 = abs(sin(time + (2.0 * pi / 3.0 * 2.0)));
    float use = t1 * one + t2 * two + t3 * three;
    float d = abs(sin(cos(time * 35.0)) * 4.0) + 5.0;
    vec3 col2 = vec3(use, use, use) * d;
    col2 /= abs(sin(time) + 2.0) * 1.0;
    col2 /= 2.0;
    col2 += rand(pos + time) * ((sin(time * 57.0) * 0.35) + 0.15);
    vec3 finalColor = col2 * intensity * (u * 5.0);
    
    gl_FragColor = vec4(finalColor, color.a);
}