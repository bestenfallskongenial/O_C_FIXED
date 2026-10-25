precision mediump float;

// Rainbow Wells
//
// Wandering centers gather circular halos and pull nearby gradients into narrow necks. As their paths cross, the surrounding field opens into broad fans and closes again around the moving wells.
//
// par_a.x - increases variation in the distance-driven color field.
// par_a.y - spreads the moving centers farther apart.
// par_a.z - changes distance attenuation, reshaping the halos and surrounding gradients.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:radial color_out:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:zoom:
float f2 = mix(0.65, 0.95, par_a[2]);   //f2:center converge:

vec3 hsv2rgb(float h,float s,float v)
{
return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}
// returns distance to point
float makePoint(vec2 x, vec2 f, vec2 s, float t)
{
   vec2 p = vec2(sin(t*f.x), cos(t*f.y)) * s * 4. *f1;
   return length(p - x) * f0;
}

vec2 minDist(vec2 mind, float i, float d)
{
   return min(mind, d) * f2;
// return min(mind, vec2(d, i)) * f2;  
}

void main( void )
{
   vec2 p=(gl_FragCoord.xy/tres.x)*2.0-vec2(1.0,tres.y/tres.x);
   float t = time*0.2;
   //float t = 10.0;
   vec2 a = vec2(1e10, 0);
   a = minDist(a, 1.0, makePoint(p, vec2(9.3,2.9), vec2(0.1,0.1), t));
   a = minDist(a, 2.0, makePoint(p, vec2(1.9,2.0), vec2(0.4,0.4), t));
   a = minDist(a, 3.0, makePoint(p, vec2(0.8,0.7), vec2(0.4,0.5), t));
   a = minDist(a, 4.0, makePoint(p, vec2(2.3,0.1), vec2(0.6,0.3), t));
   a = minDist(a, 5.0, makePoint(p, vec2(0.8,1.7), vec2(0.5,0.4), t));
   a = minDist(a, 6.0, makePoint(p, vec2(0.3,1.0), vec2(0.4,0.4), t));
   a = minDist(a, 7.0, makePoint(p, vec2(1.4,1.7), vec2(0.4,0.5), t));
   a = minDist(a, 8.0, makePoint(p, vec2(1.3,2.1), vec2(0.6,0.3), t));
   a = minDist(a, 9.0, makePoint(p, vec2(1.8,1.7), vec2(0.5,0.4), t));     
   float i = a.x * 20.0;
   vec3 rgb = hsv2rgb( atan(i,p.x),0.9,0.8);

   gl_FragColor = vec4(rgb, color.a);
}