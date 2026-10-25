precision mediump float;

// Drifting Topography
//
// Overlapping circular waves divide the surface into flat contour regions. Their boundaries form rounded islands, winding channels, and nested bands that continually rearrange like a topographic map shifting beneath its own shorelines.
//
// par_a.x - increases coordinate scale, showing more, smaller features.
// par_a.y - increases the second wave field’s radial frequency.
// par_a.z - strengthens the first wave field’s contribution.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:zoom:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:thickness:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:ring depth:

const vec3 col1 = vec3(.964705882, .990196078, .990196078);
const vec3 col2 = vec3(.266666667, .333333333, .231372549);
const vec3 col3 = vec3(.839215686, .880392157, .849019608);
const vec3 col4 = vec3(.776470588, .772549020, .729411765);
const vec3 col5 = vec3(.509803922, .592156863, .503921569);

void main( void )
{
vec2 position = gl_FragCoord.xy / tres.xy * f0 * 2.;
vec2 ca = vec2(0.1, 0.2);
vec2 cb = vec2(0.7, 0.9);
float da = distance(position, ca);
  float db = distance(position, cb) * f1;
  float t = time * 0.1;
float c1 = sin(da * cos(t) * 18.0 + t * 10.0) * f2;
float c2 = cos(position.y * 1.0 + t);
float c3 = cos(db * 19.0) + sin(t);

float p = (c1 + c2 + c3) / 3.0;
  vec4 col = vec4(0, 0, 0, 2);

  if (p <= 0.0) col.rgb = col1;
  else if (p < 0.2) col.rgb = col2;
  else if (p < 0.3) col.rgb = col3;
  else if (p < 0.4) col.rgb = col4;
  else col.rgb = col5;

gl_FragColor = vec4(col.rgb, color.a);
}