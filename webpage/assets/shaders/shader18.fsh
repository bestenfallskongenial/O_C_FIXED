// www.shadertoy.com/view/4dcSRj             
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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:rotate 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:rotate 2:
float f2 = mix(0.1, 0.95, par_a[2]);    //f2:zoom:

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy * 2. - 1.;
    uv.x *= tres.x / tres.y;
    uv = abs(uv);
    gl_FragColor = vec4(vec3(0), color.a);
    gl_FragColor.rgb += smoothstep(.2, .24, sin(uv.x + time * vec3(1, 2, 4)) + .5 - uv.y);
    gl_FragColor.rgb += smoothstep(.2, .24, sin(uv.y * 2. + time * vec3(1, 2, 4)) / 2. + 1. - uv.x);
    gl_FragColor.rgb = abs(1. - gl_FragColor.rgb);
    float angle = time * .2,
        s = sin(angle) * f0,
        c = cos(angle) * f1;
    uv *= mat2(c, -s, s, c);
    gl_FragColor *= texture2D(tex[0], abs(.5 - fract(uv)) * f2 * 2.);
    gl_FragColor.rgb *= clamp(sin(uv.x) * sin(uv.y) * 20. + sin(time) * 5., 0., 1.) + .5;
    gl_FragColor.a = color.a;
}




