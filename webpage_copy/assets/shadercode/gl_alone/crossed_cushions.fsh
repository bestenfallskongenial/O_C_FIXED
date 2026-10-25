precision mediump float;

// Crossed Cushions
//
// Crossing waves form rounded rectangular patches separated by narrow pinched seams. The patches slide and pulse as neighboring boundaries tighten and soften, resembling a padded surface flexing along its stitched divisions.
//
// par_a.x - increases the number of horizontal bands.
// par_a.y - increases the blue channel’s animated variation.
// par_a.z - strengthens subtractive shading, generally darkening the pattern.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:vert count:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:color offset:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:brightness:

void main(void)
{
    vec2 uv = tcoord.xy ;
    vec4 color_out;
    color_out = vec4(uv,0.5+f1*sin(time/ 2.5),1.0);
    vec4 final_color;  // to set alpha
    vec4 ca ;
    vec4 cb ;
    vec4 cc;
    ca = vec4(abs(sin( uv.y * 5. - (time* 1.25))) + 1.25);
    cb = vec4(abs(sin( uv.x * 10. + (time* 0.25 ))) / f2 + 1.25);
    cc = vec4(abs(cos( uv.y * f0 * 40. - (time* 0.75))) + 2.5 );
    final_color = color_out+ca-cc/cb;

    gl_FragColor = vec4(final_color.rgb, color.a);
}