precision mediump float;

// Luminance Patchwork
//
// Selected brightness regions of the source image open into a smaller, repeating version of the same picture. These tiled patches shimmer within the original image, creating a moving patchwork whose boundaries follow the picture’s light and shade.
//
// par_a.x - brightness level selected for replacement.
// par_a.y - luminance gain used when selecting regions.
// par_a.z - narrows the selected brightness range.

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
 //f0:threshold:
//f1:grey adjust:
//f2:tolerance:

float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);
// https://www.shadertoy.com/view/lt2XDz

void main(void)
{

    vec2 uv = gl_FragCoord.xy / tres.xy;
    //uv.x = 1.0 - uv.x;

    //float tolerance = 0.2 + (2.*f1-1.) * 0.1;
    float tolerance = 0.2 / f2;
        vec3 col = texture2D(tex[0],uv).xyz;
    vec3 greyMul = vec3(0.2989, 0.5870, 0.1140) *  (f1 + 0.5);
    vec3 greyCol = col * greyMul;
    float grey = greyCol.x + greyCol.y + greyCol.z;
    float threshold = 0.5 + 0.5 * f0; //sin( time * 2.0);

    if ( grey > threshold - tolerance && grey < threshold + tolerance )
{
        float u = mod(uv.x * 4.0, 1.0);
        float v = mod(uv.y * 4.0, 1.0);
        col = texture2D(tex[0],vec2(u,v)).xyz * vec3(uv,0.5+0.5*sin( time * 4.0));
    }
   gl_FragColor = vec4(col,color.a );

}//endof main



