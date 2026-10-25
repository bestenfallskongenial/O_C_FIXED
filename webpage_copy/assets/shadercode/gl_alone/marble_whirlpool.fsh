precision mediump float;

// Marble Whirlpool
//
// Repeated small distortions gather into curling bands around an inverted center. Their layered motion creates a marbled whirlpool whose broad folds break into finer ripples as they sweep through the surrounding field.
//
// par_a.x - shifts the distorted field horizontally.
// par_a.y - changes coordinate scale and shifts the distorted field vertically.

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

const float Pi = 3.1;

const int   complexity      = 32;    // More points of color_out.
const float mouse_factor    = 56.0;  // Makes it more/less jumpy.
const float mouse_offset    = 0.0;   // Drives complexity in the amount of curls/cuves.  Zero is a single whirlpool.
const float fluid_speed     = 108.0;  // Drives speed, higher number will make it slower.
const float color_intensity = 0.8;

float f0 = mix(0.1, 0.95, par_a[0]);    // f0: :
float f1 = mix(0.05, 0.95, par_a[1]);   // f1: :
float f2 = mix(0.05, 0.95, par_a[2]);   // f2: :

vec3 hsv2rgb(float h,float s,float v)
{
	return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}

void main()
{
  vec2 p=(2.0*gl_FragCoord.xy-tres)/max(tres.x,tres.y);
  p *= par_a.y*8.3+.4; p /= dot( p*par_a.xy, p/par_a.xy*2. );  // ändrom3da tweak
  #define time time*16.
  p -= 8.*par_a.xy;
  for(int i=1;i<complexity;i++)
    {
    vec2 newp=p + time*0.001;
    newp.x+=0.6/float(i)*sin(float(i)*p.y+time/fluid_speed+0.3*float(i)) + 0.5; // + par_a.y/mouse_factor+mouse_offset;
    newp.y+=0.6/float(i)*sin(float(i)*p.x+time/fluid_speed+0.3*float(i+10)) - 0.5; // - par_a.x/mouse_factor+mouse_offset;
    p=newp;
    }
  // Change the mix ratio to increase the marble feel and change the white color_out to a light blue color_out
  float mix_ratio = 0.4 * sin(3.0 * p.x) + 0.6;
  vec3 col;
  col= hsv2rgb( mix_ratio/2. + .6, .9, .9 );
  gl_FragColor = vec4(col, color.a );
}