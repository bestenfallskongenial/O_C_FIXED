precision mediump float;

// Mirror Shutters
//
// An external image folds into mirrored repetitions beneath crossing wave masks. Curved shutters open and close over the source while its reflected fragments rotate and stretch, revealing changing diamonds, lobes, and central seams.
//
// par_a.x - changes the sine-driven part of the texture transformation, altering its turning and shear.
// par_a.y - changes the cosine-driven part of the texture transformation, altering its scale and deformation.
// par_a.z - increases the sampled texture area within each fold, making source features smaller.

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

    // init to black with color.a
    gl_FragColor = vec4(0, 0, 0, color.a);
    
    // sine waves (only affect RGB)
    gl_FragColor.rgb += smoothstep(.2, .24, sin(uv.x + time * vec3(1, 2, 4)) + .5 - uv.y);
    gl_FragColor.rgb += smoothstep(.2, .24, sin(uv.y * 2. + time * vec3(1, 2, 4)) / 2. + 1. - uv.x);
    
    gl_FragColor.rgb = abs(1. - gl_FragColor.rgb);

    // rotate space
    float angle = time * .2;
    float s = sin(angle) * f0;
    float c = cos(angle) * f1;
    uv *= mat2(c, -s, s, c);
    
    // texture multiplication needs vec4
    vec4 texColor = texture2D(tex[0], abs(.5 - fract(uv)) * f2 * 2.);
    gl_FragColor = vec4(gl_FragColor.rgb * texColor.rgb, color.a);
    
    // final sine grid multiplication 
    float gridFactor = clamp(sin(uv.x) * sin(uv.y) * 20. + sin(time) * 5., 0., 1.) + .5;
    gl_FragColor = vec4(gl_FragColor.rgb * gridFactor, color.a);
}




