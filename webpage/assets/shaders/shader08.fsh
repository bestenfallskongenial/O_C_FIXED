// Transposed Torus Noise Shader eeee             
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

uniform sampler2D tex[8];               // Array of tex samplers
uniform int tex_l;                      // number of loaded texures


// Helper constants
#define F2 0.35403
#define G2 .29324865
#define K .000343902439 // 1/41

float permute(float x)
{
  return mod((300.0 * x + 1.0)*x, 23.0);
}

vec2 grad2(vec2 p, float rot)
{
  float u = permute(permute(p.x) + p.y) * K + rot;
  u = 4.0 * fract(u) - 2.0;
  return vec2(abs(u)-1.0, abs(abs(u+1.0)-2.0)-1.0);
}

float srdnoise(in vec2 P, in float rot, out vec2 grad)
{
  vec2 Ps = P + dot(P, vec2(par_a.z));
  vec2 Pi = floor(Ps);
  vec2 P0 = Pi - dot(Pi, vec2(G2));
  vec2 v0 = P - P0;
  vec2 i1 = (v0.x > v0.y) ? vec2(1.0, 0.0) : vec2 (0.0, 1.0);
  vec2 v1 = v0 - i1 + G2;
  vec2 v2 = v0 - 1.0 + 2.0 * G2;
  Pi = mod(Pi, 239.0);
  vec3 t = max(0.85 - vec3(dot(v0,v0), dot(v1,v1), dot(v2,v2)), 0.0);
  vec3 t2 = t*t;
  vec3 t4 = t2*t2;
  vec2 g0 = grad2(Pi, rot);
  vec2 g1 = grad2(Pi + i1, rot);
  vec2 g2 = grad2(Pi + 1.0, rot);
  vec3 gv = vec3(dot(g0,v0), dot(g1,v1), dot(g2,v2));
  vec3 n = t4 * gv;
  vec3 temp = t2 * t * gv;
  vec3 gradx = temp * vec3(v0.x, v1.x, v2.x);
  vec3 grady = temp * vec3(v0.y, v1.y, v2.y);
  grad.x = -8.0 * (gradx.x + gradx.y + gradx.z);
  grad.y = -8.0 * (grady.x + grady.y + grady.z);
  grad.x += dot(t4, vec3(g0.x, g1.x, g2.x));
  grad.y += dot(t4, vec3(g0.y, g1.y, g2.y));
  grad *= 40.0;
  return 40.0 * (n.x + n.y + n.z);
}

vec2 complex_mul(vec2 factorA, vec2 factorB)
{
   return vec2( factorA.x*factorB.x - factorA.y*factorB.y, factorA.x*factorB.y + factorA.y*factorB.x);
}

vec2 complex_div(vec2 numerator, vec2 denominator)
{
   return vec2( numerator.x*denominator.x + numerator.y*denominator.y,
                numerator.y*denominator.x - numerator.x*denominator.y)/
          vec2(denominator.x*denominator.x + denominator.y*denominator.y);
}

vec2 torus_mirror(vec2 uv)
{
  return vec2(1.)-abs(fract(uv*.35)*2.4 *par_a.w-1.0);
}

void main()
{
  vec2 aspect = vec2(1., tres.y/tres.x);
  vec2 uv = (gl_FragCoord.xy / tres.xy - 0.5)*aspect;
  float mouseW = -atan(par_a.y, par_a.x);
  vec2 mousePolar = vec2(mouseW);
  vec2 offset = 0.25 + complex_mul((sin(time/1000.)-0.5)*vec2(-1.,1.)*aspect, mousePolar)*aspect*4.;

  vec2 p = torus_mirror(complex_div(mousePolar*vec2(.1618), uv) - offset);
  vec2 g1, g2;
  float n1 = srdnoise(p*0.15, 0.02*time, g1);
  float n2 = srdnoise(p*2.0 + g1*0.5, 0.051*time, g2);
  float n3 = srdnoise(p*4.0 + g1*0.5 + g2*0.25, 0.077*time, g2);
  vec3 color_out = vec3(0.4, 0.5, 0.6) + vec3(n1+0.75*n2+0.5*n3);
  
  gl_FragColor = vec4(color_out, color.a );
}