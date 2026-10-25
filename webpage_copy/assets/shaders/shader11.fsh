// the beauty 2             //https://www.shadertoy.com/view/tl3fzH //https://www.glslsandbox.com/e#82699.0                                     
//--------------------------// shader name length of 26 characters

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

#define PI              3.141592654
#define TAU             (2.0*PI)
#define L2(x)           dot(x, x)
#define ROT(a)          mat2(cos(a), sin(a), -sin(a), cos(a))
#define PSIN(x)         (0.5+0.5*sin(x))
#define LESS(a,b,c)     mix(a,b,step(0.,c))
#define SABS(x,k)       LESS((.5/(k))*(x)*(x)+(k)*.5,abs(x),abs(x)-(k))

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:geometry 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:wavy pixellate:
float f2 = mix(0.05, 1.95, par_a[2]);   //f2:contrast ripples:

const float fixed_radius2 = 1.9;
const float min_radius2   = 0.5;
const float folding_limit = 1.0;
const float scale         = -2.8;

void sphere_fold(inout vec3 z, inout float dz)
{
    float r2 = dot(z, z);
    if(r2 < (min_radius2*f1))
      {
      float temp = ((fixed_radius2*f0) / (min_radius2*f1));
      z *= temp;
      dz *= temp;
      }
    else if(r2 < (fixed_radius2*f0))
      {
      float temp = ((fixed_radius2*f0) / r2);
      z *= temp;
      dz *= temp;
      }
}

vec3 polySoftMin3(vec3 a, vec3 b, vec3 k)
{
  vec3 h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0);
  return mix(b, a, h) - k*h*(1.0-h);
}

void box_fold(inout vec3 z, inout float dz)
{
  const float k = 0.05;
  // Soft clamp after suggestion from ollij
  vec3 zz = sign(z)*polySoftMin3(abs(z), vec3((folding_limit*f2)), vec3(k));
  // Hard clamp
  // z = clamp(z, -(folding_limit*f2), (folding_limit*f2));
  z = zz * 2.0 - z;
}

float sphere(vec3 p, float t)
{
  return length(p)-t;
}

float torus(vec3 p, vec2 t)
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float mb(vec3 z)
{
    vec3 offset = z;
    float dr = 1.0;
    float fd = 0.0;
    for(int n = 0; n < 5; ++n)
      {
      box_fold(z, dr);
      sphere_fold(z, dr);
      z = scale * z + offset;
      dr = dr * abs(scale) + 1.0;        
      float r1 = sphere(z, 5.0);
      float r2 = torus(z, vec2(8.0, 1));
      r2 = abs(r2) - 0.25;
      float r = n < 4 ? r2 : r1;        
      float dd = r / abs(dr);
      if (n < 3 || dd < fd)
        {
        fd = dd;
        }
      }
    return fd;
}

vec2 toPolar(vec2 p)
{
  return vec2(length(p), atan(p.y, p.x));
}

vec2 toRect(vec2 p)
{
  return vec2(p.x*cos(p.y), p.x*sin(p.y));
}

float modMirror1(inout float p, float size)
{
  float halfsize = size*0.5;
  float c = floor((p + halfsize)/size);
  p = mod(p + halfsize,size) - halfsize;
  p *= mod(c, 2.0)*2.0 - 1.0;
  return c;
}

float smoothKaleidoscope(inout vec2 p, float sm, float rep)
{
  vec2 hp = p;
  vec2 hpp = toPolar(hp);
  float rn = modMirror1(hpp.y, TAU/rep);
  float sa = PI/rep - SABS(PI/rep - abs(hpp.y), sm);
  hpp.y = sign(hpp.y)*(sa);
  hp = toRect(hpp);
  p = hp;
  return rn;
}

float weird(vec2 p)
{
  const float s = 0.55;
  p /= s;
  float rep = 20.0;
  float ss = 0.05*6.0/rep;
  vec3 p3 = vec3(p.x, p.y, PSIN(time*0.53));
  p3.yz *= ROT(time*0.1);
  float n = smoothKaleidoscope(p3.xy, ss, rep);
  return mb(p3)*s;
}
float df(vec2 p)
{
  float d = weird(p);
  return d;
}

vec3 postProcess(vec3 col, vec2 q)
{
  col=pow(clamp(col,0.0,1.0),vec3(1.0/2.2)); 
  col=col*0.6+0.4*col*col*(3.0-2.0*col);  // contrast
  col=mix(col, vec3(dot(col, vec3(0.33))), -0.4);  // saturation
  col*=0.5+0.5*pow(19.0*q.x*q.y*(1.0-q.x)*(1.0-q.y),0.7);  // vigneting
  return col;
}

void main(void)
{
  vec2 q = gl_FragCoord.xy/tres.xy;
  vec2 p = -1. + 2. * q;
  p.x *= tres.x/tres.y;
	float aa   = 1.0/tres.y;
  float d = df(p);
  vec3 col  = vec3(0.0);
  col       = mix(col, vec3(1.0), smoothstep(-aa, aa, -d)); 	
  //col = postProcess(col, q);
  gl_FragColor = vec4(col, color.a);
}
