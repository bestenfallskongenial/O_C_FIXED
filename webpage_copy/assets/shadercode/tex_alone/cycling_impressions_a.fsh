precision mediump float;

// Cycling Impressions
//
// An external image retains its shape while its tones pass through a repeating transformation. Details become shifting bands of brightness, with a radial sample from the source adding another phase to the cycling impressions.

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
 //f0::
//f1::
//f2::

float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);
// https://www.shadertoy.com/view/lltGW2

void main(void)
{

    vec2 uv = gl_FragCoord.xy / tres.xy;
    vec4 c = texture2D(tex[0], uv);
    ///gl_FragColor = c;

    float z = (texture2D(tex[0], vec2(length(uv-.5),1.)).r)*(.5/tres.x*.2+.02);

    //float zoom = (1.-z);///sin(time*20.)*.1+.9;
    //uv-=.5;
    //uv*=zoom;
    //uv+=.5;

    c.rgb = sin(c.rgb*(6.2+.5/tres.y*20.)+time+vec3(3., 1.5,.5*texture2D(tex[0], vec2(1.5*length(uv-.5),1.)).r))*.5+.5;
    c.a = color.a;
    gl_FragColor = c;

}//endof main



