precision mediump float;

// Contour Current
//
// Dense contour bands curl into oval eddies and sweeping folds. The source image runs through these ridges, repeatedly revealed and darkened as the contours expand, contract, and slip past one another.
//
// par_a.x - speed of contour expansion and contraction.
// par_a.y - reduces one group of contour-shaping waves.
// par_a.z - reduces another group of contour-shaping waves.
// par_a.w - coordinate scale of both image and contours.
// par_b.x - rotation through a full turn.

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

float f0 = mix(0.05, 1.95, par_a[0]);   // f0: :
float f1 = mix(0.3, 1.95, par_a[1]);    // f1: :
float f2 = mix(0.3, 1.95, par_a[2]);    // f2: :
float f3 = mix(0.05, 0.49, par_a[3]);   // f2:zoom:
float f4 = mix(0.05, 360.0, par_b[0]);  // f2:rotation:
 // Simplified wave function
float wave(float x, float y, float f2, float f3)
{
    return sin(10.0*x+10.0*y) / 5.0 +
           sin(20.0*x+15.0*y) / f3 +
           sin(4.0*x+10.0*y) / -f2 +
           sin(y) / 2.0 +
           sin(x*x*y*20.0) + sin(x * 20.0 + 4.0) / f3 +
           sin(y * 30.0) / 5.0 + sin(x) / f2; 
}
void main()
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
    uv *= f3;
    float f4 = radians(f4);
    uv = vec2(  uv.x * cos(f4) - uv.y * sin(f4), uv.x * sin(f4) + uv.y * cos(f4));
    mediump float z = wave(uv.x, uv.y, f1, f2) + 2.0;
    z *= 4.0 * sin(1.57 + (f0 * time) / 5.0);
    mediump float d = fract(z);
    if (mod(z, 2.0) > 16.0)         
        {
        d = 1.0 - d;
        }
    vec4 texColor1 = texture2D(tex[0], uv - vec2(0.5, 0.5));
    gl_FragColor = vec4(texColor1.rgb * d, 1.0);
    //    gl_FragColor = vec4(d, d, d, color.a);
}