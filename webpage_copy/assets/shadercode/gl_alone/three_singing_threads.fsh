precision mediump float;

// Three Singing Threads
//
// Three sine-wave traces travel across the screen at different speeds. Their paths cross and separate like vibrating strings, briefly joining into brighter intersections before drifting apart again.
//
// par_a.x - increases the red trace’s frequency, amplitude, and thickness.
// par_a.y - increases the green trace’s frequency, amplitude, and thickness.
// par_a.z - increases the blue trace’s frequency, amplitude, and thickness.

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

//f0:red frequency:
//f1:green frequency:
//f2:blue frequency:
float f0 = mix(0.1, 0.95, par_a[0]);
float f1 = mix(0.1, 0.95, par_a[1]);
float f2 = mix(0.1, 0.95, par_a[2]);


/*float cubicPulse( float c, float w, float x )
{
    x = abs(x - c);
    if( x>w ) return 0.0;
    x /= w;
    return 1.0 - x*x*(3.0-2.0*x);
}*/

float drawGraph(vec2 pos, float env, float THICKNESS) {
if ((pos.y < env + THICKNESS) && (pos.y > env - THICKNESS)) {
return 1.0;
}
return 0.0;
}

void main( void ) {
float thickness = 0.1;

vec3 color_out = vec3(0.0);
vec2 position = ( gl_FragCoord.xy / tres.xy );
position = position * vec2(2.0) - vec2(1.0);

color_out.x += drawGraph(position, sin( f0 * 16. * position.x + time * 8.0) * f0, thickness * f0); 

color_out.y += drawGraph(position, sin( f1 * 8. * position.x + time * 12.0) * f1, thickness * f1);

color_out.z += drawGraph(position, sin( f2 * 12. * position.x + time * 16.0) * f2, thickness * f2);

gl_FragColor = vec4(color_out,color.a);
}


