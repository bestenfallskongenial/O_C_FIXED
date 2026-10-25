precision mediump float;

// Twin Bellows
//
// Mirrored lobes swell around a central seam, rounding into paired lenses before opening into a narrow-waisted bow. Traveling radial bands make their boundaries flare and retreat like two bellows breathing against one another.
//
// par_a.x - changes angular shaping, altering lobe width and separation.
// par_a.y - increases radial frequency, bringing successive bands closer together.
// par_a.z - rotates the pattern through a full turn.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:x mod:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:y mod:
float f2 = mix(0.0, 1.0, par_a[2]);     //f2:rotate:

mat2 rotate(float a)
{
float c = cos(a), s = sin(a);
return mat2(c, -s, s, c);
}

void main(void)
{
vec2 uv = (2. * gl_FragCoord.xy - tres) / tres.y;
vec3 color_out = vec3(0.9);
//uv *= rotate(time * .1);
uv *= rotate(f2 * 6.28319);
uv = abs(uv);
vec2 ar = vec2(sin(atan(uv.x, uv.y)), length(uv)) * vec2(f0,f1);
color_out = mix(color_out, vec3(.4, 0., .7), sin(8. * (ar.y * 1. - time) + time) + (8. * ar.x - 8. * ar.y));
color_out = mix(color_out, vec3(.7, .2, 0.), cos(8. ) * (5. * ar.y - 8. * ar.x));
color_out /= mix(color_out, vec3(.4, 0., .7), sin(8. * (ar.y * 1. - time) + time) + (8. * ar.x - 8. * ar.y));

gl_FragColor = vec4(color_out, color.a);
}