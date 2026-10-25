precision mediump float;

// Buckled Lattice
//
// Broad crossing waves are cut by thin tangent ridges, creating a lattice of bent cells and abrupt seams. The surface stretches, flickers, and folds like a flexible grid repeatedly buckling under uneven pressure.
//
// par_a.x - strengthens the broad wave layer feeding red and green.
// par_a.y - strengthens the cross-coordinate pulse feeding green and blue.
// par_a.z - increases the frequency of the vertical tangent ridges.

 
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
 
    //f0:red squash:
//f1:horz push:
//f2:ridges:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

  
void main( void )
{

vec2 position = ( gl_FragCoord.xy / tres.xy );

float color_out = sin(position.x * 10.0 +  time) + cos(position.y * 10.0 +  time) * length(position);
float green = cos(position.x * 23.0 + sin( time)) + sin(position.y * 10.0 + sin( time)) * 20. * f0;
float blue = 10.0 * sin( time * 1.0) * cos(position.x) * position.y * position.x * f1 * 10.;
blue += tan(position.x*(cos(position.y+( time/10.0))*0.5) * 50.0) + tan(position.y*(sin(exp(position.x)+sin( time/ 13.0))*0.5) * 66.0 * f2);

gl_FragColor = vec4(green+color_out, blue+green, blue+color_out,1.0); //vec4( vec3( color_out, color_out * 0.5, sin( color_out +  time / 3.0 ) * 0.75 ), color.a );

}


