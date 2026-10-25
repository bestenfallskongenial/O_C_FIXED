precision mediump float;

// Drifting Apertures
//
// Rounded openings drift through a repeating field, gathering into diamonds before softening into larger pools. The arrangement turns, expands, and contracts beneath a gentle falloff toward its edges.
//
// par_a.x - moves the pattern’s center horizontally.
// par_a.y - moves the pattern’s center vertically.
// par_a.z - increases the reach of the animated spatial frequency, allowing more, smaller shapes.

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
 
    //f0:center x:
//f1:center y:
//f2:zoom:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);


void main(void)
{
vec2 u=(gl_FragCoord.xy/tres.x)/1.0-vec2(1.0,tres.y/tres.x) * vec2(f0,f1);

float t= time*0.35;
float tt=sin(t/5.0)*128.0*f2;
float x=u.x*tt+sin(t*2.1)*4.0;
float y=u.y*tt+cos(t*2.3)*4.0;
float c=cos(y)+sin(x);
float zoom=sin(t);

x=x*zoom/2.0+sin(t*1.1);
y=y*zoom/2.0+cos(c*1.3);

float xx=cos(t*0.7)*x-sin(t*0.7)*y;
float yy=sin(t*0.7)*x+cos(t*0.7)*y;

c=(sin(c+sin(xx)+sin(yy))+.80)*1.95;

gl_FragColor=vec4((1.0-length(u)*0.90)*vec3(c*1.4,c*1.4,c*1.4),color.a);
}


