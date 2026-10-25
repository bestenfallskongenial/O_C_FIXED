precision mediump float;

// Liquid Honeycomb
//
// A repeating cellular pattern stretches through a continuously folding distortion. Rounded openings compress into dotted chains, elongated slots, and dense clusters, like a honeycomb pulled through shifting currents.
//
// par_a.x - expands the filled regions and changes their thickness.
// par_a.y - increases cell size, reducing repetition.
// par_a.z - increases vertical coordinate scale, compressing features vertically.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:thickness:
float f1 = mix(0.2, 0.95, par_a[1]);    //f1:density:
float f2 = mix(0.2, 0.5, par_a[2]);     //f2:vert stretch:

const float PI = atan(0.0,-1.0);

float hexagonalTiles(vec2 pos, float hexaGridSize)
{
  pos *= (0.283 / hexaGridSize);
  pos.x *= sqrt(3.0) / 3.0;
  float k = 44.*(cos(pos.y)*cos(pos.x) + cos(pos.x)*cos(pos.x) - sin(PI*(max(0.1,f0) +.5)) - 1.0); 
  k = clamp (k, 0.0, 1.0);
  return k;
}

void main(void)
{
vec2 pos = vec2(10.0, f2 * 20.) *(gl_FragCoord.xy / tres.xy);
pos.x *= tres.x/tres.y + min(0.3, f0);
pos = (pos*vec2(cos(pos.x+time*1.1),sin(pos.y+time*0.99)))*sin(pos+cos(time));
float k = hexagonalTiles(6.0*pos, f1);
vec3 colorout = vec3(1.0, 1.0, 1.0);

gl_FragColor = vec4(k*colorout, color.a );
}
