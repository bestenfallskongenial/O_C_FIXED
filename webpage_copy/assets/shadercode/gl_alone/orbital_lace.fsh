precision mediump float;

// Orbital Lace
//
// Repeated fractal folds gather into delicate rings, small linked structures, and a fine central thread. Their rotating slice continually reshapes the arrangement, while soft glow and projected shadows make the lace appear suspended above a darker surface.
//
// par_a.z - changes the fractal slice and the spread of its glow.

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

float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 3.5, par_a[2]);

// Emulate some GLSL ES 3.x
float tanh(float x)
{
    float ex = exp(2.0 * x);
    return ((ex - 1.) / (ex + 1.));
}

#define PI              3.141592654
#define TAU             (3.0*PI)
#define L2(x)           dot(x, x)
#define ROT(a)          mat2(cos(a), sin(a), -sin(a), cos(a))
#define PSIN(x)         (0.25+0.5*sin(x))

vec3 hsv2rgb(vec3 c)
{
  const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

float apollian(vec4 p, float s)
{
  float scale = 1.5;
  for(int i=0; i<6; ++i)
    {
    p        = -1.0 + 2.0*fract(0.6*p+0.5);
    float r2 = dot(p,p);
    float k  = s/r2;
    p       *= k;
    scale   *= k;
    }
  
  return abs(p.y)/scale;
}

float weird(vec2 p)
{
    // Basic rotation of input coordinates
    p *= ROT(time * 0.1);
    
    // Simple 4D point construction
    vec4 pp = vec4(p.x, p.y, 0.0, 0.1);  // Fixed small w component
    
    // Single rotation for movement
    pp.yz *= ROT(time * 0.2);
    
    // Apply zoom from uniform
    pp *= f2;  // f2 is your zoom uniform
    
    float d = apollian(pp, 1.2);
    return d;
}

float df(vec2 p)
{
  float zoom = f2;
  p /= zoom;
  float d0 = weird(p);
  return d0*zoom;
}

vec3 color_out(vec2 p)
{
  float aa = 7.0/tres.y;
  const float lw = 0.0235;
  const float lh = 1.25;
  // Keep only the first light position
  const vec3 lp1 = vec3(0.5, lh, 0.5);
  
  // Calculate distance field once and cache it
  float d = df(p);
  
  float b = -0.125;
  float t = 10.0;
  vec3 ro = vec3(0.0, t, 0.0);
  vec3 pp = vec3(p.x, 0.0, p.y);
  vec3 rd = normalize(pp - ro);
  vec3 ld1 = normalize(lp1 - pp);
  
  float bt = -(t-b)/rd.y;
  vec3 bp = ro + bt*rd;
  vec3 srd1 = normalize(lp1-bp);
  float bl21 = L2(lp1-bp);
  float st1 = (0.0-b)/srd1.y;
  vec3 sp1 = bp + srd1*st1;
  
  // Single shadow calculation
  float sd1 = df(sp1.xz);
  
  vec3 col = vec3(0.0);
  const float ss = 15.0;
  // Single light contribution
  col += vec3(1.0, 1.0, 1.0)*(1.0-exp(-ss*(max((sd1+0.0*lw), 0.0))))/bl21;
  
  float l = length(p);
  float hue = fract(0.75*l-0.3*time)+0.3+0.15;
  float sat = 0.75*tanh(2.0*l);
  vec3 hsv = vec3(0.8, 1.0, 0.47);
  vec3 bcol = hsv2rgb(hsv);
  
  col *= (1.0-tanh(0.75*l))*0.5;
  col = mix(col, bcol, smoothstep(-aa, aa, -d));  
  col += 0.5*sqrt(bcol.zxy)*(exp(-(10.0+100.0*tanh(l))*max(d, 0.0)));
  
  return col;
}

vec3 postProcess(vec3 col, vec2 q)
{
  col=pow(clamp(col,0.0,1.0),vec3(1.0/2.2)); 
  //col=col*0.1+0.6*col*col*(3.0-2.0*col);  // contrast
  //col=mix(col, vec3(dot(col, vec3(0.33))), -0.9);  // saturation
  //col*=0.5+0.5*pow(19.0*q.x*q.y*(1.0-q.x)*(1.0-q.y),0.7);  // vigneting
  return col;
}

// void mainImage(out vec4 fragColor, in vec2 fragCoord)
void main() 
{
  vec2 q = gl_FragCoord.xy/tres.xy;
  vec2 p = -1. + 2. * q;
  p.x *= tres.x/tres.y;
  vec3 col = color_out(p);
  //col = postProcess(col, q);
  gl_FragColor = vec4(col, color.a);
}

// void main(void)
// {
//    mainImage(gl_FragColor, gl_FragCoord.xy);
// }