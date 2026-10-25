precision mediump float;

// Turning Fractal Veils
//
// Several fractal slices overlap as they turn through space. Their broad silhouettes open into curved sheets and narrow fringes, then fold across one another like translucent veils rotating around a shared axis.
//
// par_a.x - raises the escape threshold, changing the fractal boundaries and surrounding shading.
// par_a.y - changes the fractal parameter shared by the rotating slices.
// par_a.z - changes the cosine-driven part of the rotations, altering compression and deformation.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:accent lines:
float f1 = mix(0.2, 0.8, par_a[1]);     //f1:spatial pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:squeeze:

#define ITER 4
#define ITERZ 4

float escRadius= 0.5  * f0 ;
float ratio=tres.x/tres.y;

mat4 rotationMatrix(vec3 axis, float angle)
{
axis = normalize(axis);
float s = sin(angle);
float c = cos(angle) * f2;
float oc = 1.0 - c;
return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
    oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
    oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
    0.0, 0.0, 0.0, 1.0);
}

vec4 julia(vec2 z,vec2 c)
{
float x=0.0,y=0.0;
float iNorm;
float zAbs;
int iter=0;
vec4 color_out=vec4(1.0);
for(int i=0;i<ITER;i++)
    {
    x=(z.x*z.x-z.y*z.y)+c.x;
    y=(z.y*z.x+z.x*z.y)+c.y;
    z.x=x;
    z.y=y;
    iter++;
    if((x*x+y*y)>escRadius + f0)
        {
        break;
        }
    }
#define MUL 0.25
zAbs=sqrt(x*x+y*y);
iNorm=float(iter+1)-log(log(zAbs))/log(2.0);
color_out.r=sin(iNorm*MUL-0.15);
color_out.g=sin(iNorm*MUL);
color_out.b=sin(iNorm*MUL+0.15);
return color_out;
}

void main(void)
{
float step=2.0/float(ITERZ);
vec4 c=vec4(0.0);
vec3 p=vec3(gl_FragCoord.xy.x/tres.x*ratio,gl_FragCoord.xy.y/tres.y,0.0);
vec3 pRot;
p.xy-=vec2(0.5*ratio,0.5);
p.z=-1.0;
p.xy*=3.0;
float t=cos(time/2.0)*1.000001;
for(int i=0;i<ITERZ;i++)
    {
    pRot=(vec4(p,1.0)   *rotationMatrix(vec3(1.0,0.0,0.0),time/5.0)
                        *rotationMatrix(vec3(0.0,0.0,1.0),time/5.0)      
                        ).xyz;
    c+=julia(pRot.xy,vec2(pRot.z,2.0*(f1*2.0-1.000001)));
    p.z+=step;
}
    gl_FragColor = vec4(smoothstep(0.0, 1.0, float(c) / float(ITERZ)),                                  // gray
                        smoothstep(0.0, 1.0, float(c) / float(ITERZ)), 
                        smoothstep(0.0, 1.0, float(c) / float(ITERZ)), color.a);

//  vec3 color_out = smoothstep(0.0, 1.00, c.rgb / float(ITERZ));                                       // color
//  gl_FragColor = vec4(color_out, color.a);

//  gl_FragColor=smoothstep(0.0,1.00,c/float(ITERZ));                                                   // color original
//  gl_FragColor.a=color.a;
}

