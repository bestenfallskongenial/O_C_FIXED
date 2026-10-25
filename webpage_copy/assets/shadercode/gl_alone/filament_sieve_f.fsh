precision mediump float;

// Woven Reflections
//
// Broad curved regions are crossed by dense meshes of fine lines and broken marks. Their boundaries shift through small phase changes, making the surface resemble reflections stretched across overlapping woven sheets.
//
// par_a.x - increases horizontal pattern frequency.
// par_a.y - increases vertical pattern frequency.
// par_a.z - reduces red intensity while raising the green-channel baseline.

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
 
    //f0:horz stretch:
//f1:vert stretch:
//f2:color_out changes:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

  //just testing and learning :)

void main( void )
{


vec2 position = ( gl_FragCoord.xy / tres.xy )  * vec2(f0, f1) * 8. ;
float depth = gl_FragCoord.z;

float color_out = 0.5;

float my_mod = 5.;

//color_out += mod( time, 0.1);
//color_out += dot(position.x, position.y)*depth;

color_out *= abs(mod(position.x*my_mod*sin(position.x)*position.y+cos( time), position.x*my_mod*cos(position.x)*position.x+sin( time)));
color_out *= abs(mod(position.y*my_mod*cos(position.y)*position.x+sin( time), position.y*my_mod*sin(position.y)*position.y+cos( time)));

gl_FragColor = vec4(color_out*0.2 / f2, color_out*0.1 + f2, color_out, color.a );
}


