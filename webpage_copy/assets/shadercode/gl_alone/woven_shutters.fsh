precision mediump float;

// Woven Shutters
//
// Horizontal and vertical wave fields alternate between narrow strips and intersecting rectangular patches. Their visibility pulses with time, opening and closing portions of the weave like shutters moving across a softly shaded surface.
//
// par_a.x - changes the frequency of the rectangular bands.
// par_a.y - changes the frequency of the smoother wave shaping.
// par_a.z - changes the balance between the horizontal and vertical fields.

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

vec2 mouse = par_a.xy;

float f0 = mix(0.2, 0.95, par_a[0]);    //f0:checker size:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:wave size:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:color rotate:

vec3 hsv2rgb(float h,float s,float v)
{
return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}

float smoothrect( float f)
{
    float r = sign(sin( f * f0));
    float s = sin( f * f1);
    return mix(r,s, 1.0 - abs(s));
}

void main( void )
{
float aspect = tres.x /tres.y;
vec2 unipos = ( gl_FragCoord.xy / tres.xy );
unipos.x *=aspect;

float sint = sin(time);
float cost = cos(time) * f2;
float size = 32.0;

float a = sint * smoothrect(  unipos.x *size);
float b = cost * smoothrect(  unipos.y *size );
float shade = mix(a,b, pow(sint*4.0,cost));
shade = clamp(shade,0.0,0.45);
shade *= unipos.x*2.0;

gl_FragColor = vec4(hsv2rgb(shade, 0.86, shade*2.0), color.a); 
}