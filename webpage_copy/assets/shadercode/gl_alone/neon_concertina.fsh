precision mediump float;

// Neon Concertina
//
// Three glowing wave traces weave across a narrow horizontal band. Their crests bunch together, stretch apart, and cross like the folds of a concertina, with bright knots gathering wherever the curves meet.
//
// par_a.x - increases wave frequency in steps.
// par_a.y - increases wave amplitude.
// par_a.z - increases the horizontal bending and compression of the waves.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:radial color:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:zoom:
float f2 = mix(0.65, 0.95, par_a[2]);   //f2:center converge:


#define PI 3.141592

vec3 pal(float x){
   return max(min(sin(vec3(x,x+PI*2.0/3.0,x+PI*4.0/3.0))+0.5,1.0),0.0);
}

void main( void ) {
   
   float t = time*0.9;

   
   vec2 position = ( gl_FragCoord.xy / tres.xy ) - 0.5;
   position *= 2.25;
   position.y *= dot(position,position);
   
   position.y *= 1.0+sin(position.x*3.0)*0.2;
   
   float foff = 0.93;
   float den = 0.05;
   float amp = f1;
   float freq = 5.0+ floor(f0 * 10.0);
   float offset = 0.1-tan(position.x*0.5)*5.05 * f2;

        float modifer = 0.;
   
   for(float i = 0.0; i < 3.0; i+=1.0)
      modifer += 6.0/abs((position.y + (amp*sin(((position.x*4.0 + t) + offset) *freq+i*foff))))*den;;
   
   vec3 colour = pal(position.x*2.)*0.25* modifer;
   
   gl_FragColor = vec4( colour, color.a );
   
}