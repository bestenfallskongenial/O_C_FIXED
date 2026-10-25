// by Trisomie21 // modified by @hintz
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

float f0 = mix(0.3, 0.5, par_a[0]);     //f0:scale:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:color step:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:length:
 
void main(void)
{
float scale = f0 * 15.0;
float ring = 20.0 * f1 + 20.;
float radius = tres.x*1.0;
float gap = scale*.7;
vec2 pos = gl_FragCoord.xy - tres.xy * 0.5;
float d = length(pos) * (f2 * 2.);
// Create the wiggle
d += (sin(pos.y*0.25/scale+time)*sin(pos.x*0.25/scale+time*.5))*scale*5.0;
// Compute the distance to the closest ring
float v = mod(d + radius/(ring*2.0), radius/ring);
v = abs(v - radius/(ring*2.0));
v = clamp(v-gap, 0.0, 1.0);
d /= radius;
vec3 m = fract((d-1.0)*vec3(ring*-.5, -ring, ring*.25)*0.5);

gl_FragColor = vec4(m*v, color.a);
}