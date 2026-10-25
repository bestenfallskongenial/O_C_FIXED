precision mediump float;

// Threshold Weave
//
// An external image becomes a field of fine horizontal and vertical stripes. Its red-channel threshold chooses which direction appears in each region, turning the source into a woven silhouette whose line thickness changes across the screen.
//
// par_a.x - changes the horizontal gradient controlling vertical-stripe coverage.
// par_a.y - changes the vertical gradient controlling horizontal-stripe coverage.
// par_a.z - sets the source red-channel threshold that switches between stripe directions.

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


float f0 = mix(0.05, 0.4995, par_a[0]);  //f0:horz mod:
float f1 = mix(0.5, 0.95, par_a[1]);    //f1:vert mod:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:cut off:

void main(void)
{
    vec2 uv = (gl_FragCoord.xy / tres.xy);
    vec4 tex = texture2D(tex[0], uv);

//  float vcol = v_thick*(mod(gl_FragCoord.y/12.,1.)-.45*f2) *f0;
//  float hCol = h_thick*(mod(gl_FragCoord.x/12.,1.)-.55*f2) *f1;

    float vcol = 20.*(mod(gl_FragCoord.y/5.,1.)-uv.y * f1); // / (2. * f1 - 1.);
    float hCol = 20.*(mod(gl_FragCoord.x/5.,1.)-uv.x * f0); // / (2. * f0 - 1.);

    float gray = hCol;

//  if(tex.r>0.5)

    if(tex.r> f2 )
{
      gray = vcol;
    }
    gl_FragColor = vec4(vec3(gray),color.a );


}//endof main




