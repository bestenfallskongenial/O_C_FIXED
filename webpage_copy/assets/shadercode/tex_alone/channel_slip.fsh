precision mediump float;

// Channel Slip
//
// An external image separates into three displaced channel views. Their edges slide in different directions, breaking familiar shapes into offset outlines and overlapping fragments, like several imperfectly aligned transparencies laid over the same picture.
//
// par_a.x - shifts red sampling right and blue sampling down.
// par_a.y - shifts red sampling up and green sampling right.
// par_a.z - shifts green sampling down and blue sampling left.

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

float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy - .5;
    float amp = 0.5;
    float rAmt = 2. * f0 - 1. * amp ;
    float gAmt = 2. * f1 - 1. * amp;
    float bAmt = 2. * f2 - 1. * amp;

     vec3 col;
    col.r = texture2D( tex[0], vec2(uv.x + rAmt, uv.y + gAmt) ).r;
    col.g = texture2D( tex[0], vec2(uv.x + gAmt, uv.y - bAmt) ).g;
    col.b = texture2D( tex[0], vec2(uv.x - bAmt, uv.y - rAmt) ).b;
    
    gl_FragColor = vec4(col,color.a );
}



