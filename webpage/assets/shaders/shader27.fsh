// Julia Set Shader                                     
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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:mod 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:mod 2:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:mod 3:

const float iterations = 16.0;

void main(void)
{
    vec2 position = 1.3 * vec2( (gl_FragCoord.x - tres.x * 0.5) / tres.y * 2.0,
                                (gl_FragCoord.y - tres.y * 0.5) / tres.y * 2.0  );

    float color_out = 0.0;
    float zr = position.x;
    float zi = position.y;
    float tr = (f0 + sin(time / 100.0) * 0.05) * cos(time / 10.0);
    float ti = (f1 + sin(time / 100.0) * 0.05) * sin(time / 10.0);
    float cr = tr - tr * tr + ti * ti;
    float ci = ti - 2.0 * tr * ti;
    
    for (float i = 0.0; i < iterations; i++)
        {
        float tempr = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi - ci * f2;
        zr = tempr;
        
        if ((zr * zr + zi * zi) > 4.0)
            {
            color_out = 1.0 - (i / iterations);
            break;
            }
        }

    gl_FragColor = vec4(vec3(color_out), color.a);
}