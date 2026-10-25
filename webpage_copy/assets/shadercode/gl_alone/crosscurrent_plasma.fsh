precision mediump float;

// Crosscurrent Plasma
//
// Broad wave fields cross at different angles, swelling into rounded patches before stretching into narrow channels. Their overlapping currents continuously rearrange the surface, like several soft membranes sliding through one another.
//
// par_a.x - reduces the red field’s oscillation strength.
// par_a.y - reduces the green field’s oscillation strength.
// par_a.z - reduces the blue field’s oscillation strength.

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
 
    //f0:red blur:
//f1:green blur:
//f2:blue blur:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);


  
void main( void )
{

vec2 p = 2.0 * gl_FragCoord.xy / tres.xy - 1.0;

float r = 0.75 + sin( p.x ) * ( cos( 5.0  * p.x +  time ) + sin( 7.0 * p.y -  time ) + sin(  time ) ) / f0;
float g = 0.25 + sin( p.y ) * ( cos( 3.0  * p.y +  time ) + sin( 9.0 * p.x -  time ) - cos(  time ) ) / f1;
float b = 0.75 + sin( p.x ) * ( cos( 11.0 * p.x +  time ) + sin( 3.0 * p.y +  time ) + cos(  time ) ) / f2;

gl_FragColor = vec4( r, g, b, color.a );

}


