precision mediump float;

// Sliding Stencils
//
// Irregular patches slide diagonally across an underlying square lattice. Soft transitions meet abruptly clipped edges, letting small shapes emerge, expand, and disappear like stencils passing over a dark surface.
//
// par_a.x - changes interpolation strength, altering patch shapes, contrast, and clipping.
// par_a.y - changes one corner’s contribution within each noise cell.
// par_a.z - increases pattern density, making the patches smaller.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers  uniform int tex_l;     
    
  //f0:arc blur:
//f1:arc offset:
//f2:zoom:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

 
float random(vec2 st)
{
   return fract(sin(dot(st, vec2(12.4352, 31.682))) * 45649.421);
}

float cubic(float v)
{
   return v * v * (80.0 * f0 - 2.0 * v);
}

float valuenoise(vec2 st)
{
      float v1 = random(floor(st));
   float v2 = random(floor(st) + vec2(1.0, 0.0));
   float v3 = random(floor(st) + vec2(0.0, 1.0));
   float v4 = random(floor(st) + vec2(1.0, 1.0));
      //return mix(mix(v1, v2, fract(st.x)), mix(v3, v4,  fract(st.x)), fract(st.y));
   //return mix(mix(v1, v2, smoothstep(0.0, 1.0, fract(st.x))), mix(v3, v4, smoothstep(0.0, 1.0, fract(st.x))), smoothstep(0.0, 1.0, fract(st.y)));
   return mix(mix(v1, v2, cubic(fract(st.x))), mix(v3 *f1, v4, cubic(fract(st.x))), cubic(fract(st.y)));  }
void main(void)
{
   vec2 p = gl_FragCoord.xy / tres.yy;
      float v = valuenoise(p  * 10.0 * f2 +  time);         gl_FragColor = vec4(vec3(v), color.a ); 
}//endof main


