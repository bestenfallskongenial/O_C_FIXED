//  water turbulence effect by joltz0r      
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

uniform sampler2D tex[8];               // Array of texture samplers       
uniform int tex_l;                      // number of loaded texures 

float f0 = mix(0.05, 0.95, par_a[0]);   //  f0:brightness:
float f1 = mix(0.05, 0.95, par_a[1]);   //  f1:ripple speed:
float f2 = mix(0.05, 0.95, par_a[2]);   //  f2:num ripples:

#define MAX_ITER 3

void main(void)
    {
    vec2 p = tcoord * 8.0 - vec2(20.0);
    vec2 i = p;
    float c = 1.0 * f0;
    float inten = 0.05;

    for (int n = 0; n < MAX_ITER; n++) 
        {
        float t = time * (f1 * 0.025 - (3.0 / float(n+1)));
        i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + f2 * 20.0 * cos(t + i.x));
        c += 1.0 / length(vec2(p.x / (sin(i.x+t)/inten), p.y / (cos(i.y+t)/inten)));
        }
    c /= float(MAX_ITER);
    c = 1.5 - sqrt(c);
    vec3 color_out = vec3(c * c * c * c);
    gl_FragColor = vec4(color_out, color.a);
}