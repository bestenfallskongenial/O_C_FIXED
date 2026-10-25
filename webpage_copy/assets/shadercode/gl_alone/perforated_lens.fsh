precision mediump float;

// Perforated Lens
//
// A rippling field of rounded apertures alternates between broad slots and densely packed openings. Surface shading and concentrated highlights give the changing patches a curved, lens-like appearance, as though a perforated sheet were flexing beneath a fixed viewpoint.
//
// par_a.x - moves the light direction horizontally.
// par_a.y - moves the light direction vertically.
// par_a.z - expands the shaded region and tightens the specular highlight.

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

vec2 mouse = par_a.xy;

//f0:blue horz:
//f1:blue vert:
//f2:blue total:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

const vec3 viewer = vec3(0.0, 0.0, 1.0);
const vec3 diffuseColor = vec3(0.0, 0.5, 1.0);
const float diffusePower = .5;

void main( void ) 
{
vec3 light = normalize(vec3(vec2(.5, .5)-vec2(f0, f1), 0.25));
vec2 position = ( gl_FragCoord.xy / tres.xy );

 float r = .5*sin(gl_FragCoord.x/tres.x*100.0*sin(time*.25))*cos(gl_FragCoord.y/tres.y*50.0);
 vec2 diff2D = position-vec2(.5, .5);
diff2D.x *= tres.x/tres.y;
 if (length(diff2D) < r +f2) {
float z = sqrt(r*r - (diff2D.x*diff2D.x + diff2D.y*diff2D.y));
  // 3d position: vec3 diff3D = vec3(diff2D, z);
vec2 dz = -diff2D.xy/z;
vec3 normal = normalize(vec3(dz, 1.0));

float i = dot(light, normal);
 
                // CALCULATE THE DIFFUSE LIGHT FACTORING IN LIGHT COLOUR, POWER AND THE ATTENUATION
                vec3 diffuse = i * diffusePower * diffuseColor; 
 
                //CALCULATE THE HALF VECTOR BETWEEN THE LIGHT VECTOR AND THE VIEW VECTOR. 
      //THIS IS CHEAPER THAN CALCULATING THE ACTUAL REFLECTIVE VECTOR
                vec3 h = normalize(light + viewer);
 
                // INTENSITY OF THE SPECULAR LIGHT
                // DOT PRODUCT OF NORMAL VECTOR AND THE HALF VECTOR TO THE POWER OF THE SPECULAR HARDNESS
                i = pow(dot(normal, h), 16.0 * f2);
 
                // CALCULATE THE SPECULAR LIGHT FACTORING IN LIGHT SPECULAR COLOUR, POWER AND THE ATTENUATION
                vec3 specular = vec3(i, i, i); 

  gl_FragColor = vec4(specular + diffuse, color.a);
 } else {
 gl_FragColor = vec4( 1.0, 1.0, 1.0, color.a );
 }
}


