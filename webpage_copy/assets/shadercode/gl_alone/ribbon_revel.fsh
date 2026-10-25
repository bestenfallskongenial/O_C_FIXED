precision mediump float;

// Ribbon Revel
//
// Eight ribbons bend and weave across a rotating field. Their paths sway together, gathering into bright overlaps before drifting apart like streamers caught in a turning breeze.
//
// par_a.x - ribbon thickness.
// par_a.y - magnification; higher values make the ribbons appear larger.
// par_a.z - shifts the phase of the ribbon arrangement.

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
 
    //f0:thickness:
//f1:zoom:
//f2:wiggle:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);


  // bars - thygate@gmail.com // rotation and color_out mix modifications by malc (mlashley@gmail.com) // waves modified by @hintz 2013-05-01

vec2 position;
float c = cos( time*0.5);
float s = sin( time*0.5);
mat2 R = mat2(c,-s,s,c);

float barsize = 0.15 * f0;
float barsangle = 200.0*sin( time*0.0001);

vec4 bar(float pos, float r, float g, float b)
{
//pos*=3.0;
return max(0.0, 4.0-fract(pos - position.y + 0.5*sin(2.0* time+sin(1.0-pos*2.0)*4.0*position.x)) / barsize) * vec4(r, g, b, 1.0);
}

void main(void)
{
position = (gl_FragCoord.xy - 0.5*tres.xy) / tres.xx;
position = (1.5 / f1 +0.3)  *position * R; 
float t =  time*0.05 + (f2 * 5.5);

vec4 color_out = bar(sin(t), 1.0, 0.0, 0.0);
color_out += bar(sin(t+barsangle*2.), 1.0, 0.5, 0.0);
color_out += bar(sin(t+barsangle*4.), 1.0, 1.0, 0.0);
color_out += bar(sin(t+barsangle*6.), 0.0, 1.0, 0.0);
color_out += bar(sin(t+barsangle*8.), 0.0, 1.0, 1.0);
color_out += bar(sin(t+barsangle*10.), 0.0, 0.0, 1.0);
color_out += bar(sin(t+barsangle*12.), 0.5, 0.0, 1.0);
color_out += bar(sin(t+barsangle*14.), 1.0, 0.0, 1.0);

gl_FragColor = normalize(color_out)*1.5;
}


