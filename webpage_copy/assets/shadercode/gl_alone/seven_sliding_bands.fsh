precision mediump float;

// Seven Sliding Bands
//
// Seven horizontal strips carry smoothly repeating gradients across the screen. Each follows a different channel relationship, while softened borders and shading give the rows a rounded appearance, like illuminated bands sliding beside one another.
//
// par_a.x - shifts the relative phases of the palette channels.
// par_a.y - reduces palette amplitude, softening contrast and clipping.
// par_a.z - increases palette repetition and apparent scrolling speed.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers  uniform int tex_l;         
  //f0:offset:
//f1:blur:
//f2:scroll:

float f0 = mix(0.4, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.75, 0.95, par_a[2]);

// http://glslsandbox.com/e#39994.0

 
#define PI 3.14159265359
#define TWO_PI 6.28318530718

vec3 colorA = vec3(f0,0.0,1.0);
vec3 colorB = vec3(1,f1,1);
vec3 colorC = vec3(1.0,0.0,f2);

vec3 hsb2rgb(in vec3 c)
{
vec3 rgb = clamp( abs( mod(c.x * 6.0 + vec3(0.0,4.0,2.0), 6.0) -3.0) - 1.0, 0.0, 1.0);
return rgb;
}


vec3 hsb2rgb_smooth(in vec3 c)
{
vec3 rgb = clamp( abs( mod(c.x * 6.0 + vec3(0.0,4.0,2.0), 6.0) -3.0) - 1.0, 0.0, 1.0);
rgb = rgb * rgb * (3.0-2.0*rgb);
return rgb;

}

vec3 palette( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d )
{
d/= f0 * .8;
b/= f1;
c*= f2;
    return a + b*cos( 6.28318*(c*t+d) );
}
void main(void)
{

vec2 uv = gl_FragCoord.xy / tres.xy;

vec2 center = vec2(0.5)-uv;
float angle = atan(center.y, center.x);
float rad = length(center) / 2.0;

uv.x += 0.03 *  time;

//vec3 color_out = palette(angle/TWO_PI, vec3(0.5), vec3(0.5), vec3(1.0), vec3(0., 0.33, 0.67));
vec3 color_out;
 color_out = palette(uv.x, vec3(0.5), vec3(0.5), vec3(1.0), vec3(0., 0.33, 0.67));
if(uv.y > (1.0/7.0)) color_out = palette(uv.x, vec3(0.5), vec3(0.5), vec3(1.0), vec3(0., 0.10, 0.20));
if(uv.y > (2.0/7.0)) color_out = palette(uv.x, vec3(0.5), vec3(0.5), vec3(1.0), vec3(0.3, 0.2, 0.2));
if(uv.y > (3.0/7.0)) color_out = palette(uv.x, vec3(0.5), vec3(0.5), vec3(1.0,1.0,0.5), vec3(0.8, 0.9, 0.3));
if(uv.y > (4.0/7.0)) color_out = palette(uv.x, vec3(0.5), vec3(0.5), vec3(1.0,0.7,0.4), vec3(0.0, 0.15, 0.20));
if(uv.y > (5.0/7.0)) color_out = palette(uv.x, vec3(0.5), vec3(0.5), vec3(2.0,1.0,0.0), vec3(0.5, 0.2, 0.25));
if(uv.y > (6.0/7.0)) color_out = palette(uv.x, vec3(0.8,0.5,04), vec3(0.2,0.4,0.2), vec3(2.0,1.0,1.0), vec3(0.0, 0.25, 0.25));

//band
float f = fract(uv.y * 7.0);
color_out *= smoothstep(0.50, 0.46, abs(f-0.5));

//shadow
color_out *= 0.5 + 0.5* sqrt(4.0*f*(1.0-f));

vec3 hsl = vec3(uv.x, 1.0, uv.y);
//vec3 rgb_o = hsb2rgb(hsl);
//vec3 rgb_o = hsb2rgb_smooth(hsl);
//vec3 color_out = hsb2rgb_smooth( vec3( ( (angle/TWO_PI)) + 0.5, rad, 1.0));


gl_FragColor = vec4(color_out,color.a );
   }//endof main


