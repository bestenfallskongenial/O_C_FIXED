// www.shadertoy.com/view/Xs2GDd             
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

uniform sampler2D tex[8];               // Array of tex samplers  uniform int tex_l;     
    
#define PI 3.1415926535897932384626
  // for easier glslsandbox.com conversion

vec3 col1 = vec3(0.216, 0.471, 0.698); // blue
vec3 col2 = vec3(1.00, 0.329, 0.298); // yellow
vec3 col3 = vec3(0.867, 0.910, 0.247); // red

float f0 = mix(0.3, 0.95, par_a[0]);    //f0:bulge zoom 1:
float f1 = mix(0.3, 0.95, par_a[1]);    //f1:bulge zoom 2:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:red thickness:

float disk(vec2 r, vec2 center, float radius)
{
return 1.0 - smoothstep( radius-0.008, radius+0.008, length(r-center));
}

void main(void)
{
float t =  time*2.;
//vec2 r = (2.0*tcoord.xy) / tres.y;
vec2 r = (2.0*tcoord.xy) * f0;
r *= 1.0 + 0.05*sin(r.x*5.+ time) + 0.05*sin(r.y*3.+ time);
r *= 1.0 + 0.2*length(r);
float side = f1;
vec2 r2 = mod(r, side);
vec2 r3 = r2-side/2.;
float i = floor(r.x/side)+2.;
float j = floor(r.y/side)+4.;
float ii = r.x/side+2.;
float jj = r.y/side+4.;

vec3 pix = vec3(1.0);

float rad, disks;

rad = 0.15 + f2 * .5 *sin(t+ii*jj);
disks = disk(r3, vec2(0.,0.), rad);
pix = mix(pix, col2, disks);

float speed = 2.0;
float tt =  time*speed+0.1*i+0.08*j;
float stopEveryAngle = PI/2.0;
float stopRatio = 0.7;
float t1 = (floor(tt) + smoothstep(0.0, 1.0-stopRatio, fract(tt)) )*stopEveryAngle;

float x = -0.07*cos(t1+i);
float y = 0.055*(sin(t1+j)+cos(t1+i));
rad = 0.1 + 0.05*sin(t+i+j);
disks = disk(r3, vec2(x,y), rad);
pix = mix(pix, col1, disks);

rad = 0.2 + 0.05*sin(t*(1.0+0.01*i));
disks = disk(r3, vec2(0.,0.), rad);
pix += 0.2*col3*disks * sin(t+i*j+i);

pix -= smoothstep(0.3, 5.5, length(r));
gl_FragColor = vec4(pix,color.a );
}


