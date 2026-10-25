precision mediump float;

// Fourfold Undertow
//
// Four moving centers pull overlapping wave fields into curling arms and broad winding channels. Their interference continually rearranges the surface, opening pinwheel-like folds that stretch outward before gathering around another center.
//
// par_a.x - increases radial wave frequency.
// par_a.y - increases radial wavelength, spreading the wave contours farther apart.
// par_a.z - increases coordinate scale, making the moving structures smaller.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:mod 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:mod 2:
float f2 = mix(0.2, 0.95, par_a[2]);    //f2:mod 3:

const float pi=3.1415926535;

vec3 color1=vec3(1.0,0.0,0.0);
vec3 color2=vec3(1.0,1.0,0.0);
vec2 size=tres;
float scale=1.0;

const int num=4;

float wave(vec2 pos,float angle,float wavelength,float phase,int i)
{
return sin(dot(pos,vec2(cos(angle),sin(angle)))*2.0*pi/wavelength+phase+float(i));
}

float wave2(vec2 pos,int i,float ls)
{
vec2 me = vec2(size.x*0.4*cos(0.0737*time*(float(i+1)+0.34)+float(i)),size.y*0.4*sin(0.0876*time*(float(i+1)+0.56)+float(i)));
vec2 diff = pos*f2*2. - me;
float angle2 = atan(diff.y, diff.x); //cos(atan(diff.y, diff.x) + float(i)*pi/float(num) + 0.7*time);
float dist = length(diff) * f0;
//return sin(dot(pos,vec2(cos(angle),sin(angle)))*2.0*pi/(wavelength)+phase+float(i));
return (3.0+float(i*2))*angle2+20.0*sin(6.0*dist/ls)*sin(0.5*time+(float(i+3)*2.34))+time;
}

void main()
{
vec2 pos=gl_FragCoord.xy/scale-size/2.0;
float amp=0.0;
float ls = length(size) * f1;
for(int i=0;i<num;i++)
    {
    amp+=wave2(pos,i,ls);
    }
float y=gl_FragCoord.y/size.y;
float c=sin(amp);//sin(pi*12.0*(amp+float(num))/float(num)/2.0);
c=clamp(c,0.0,1.0);
c=pow(c,0.2);
vec3 colorout=color1+(color2-color1)*clamp((c+y)*0.6, 0.0, 1.0);

gl_FragColor=vec4(colorout,color.a);
}


