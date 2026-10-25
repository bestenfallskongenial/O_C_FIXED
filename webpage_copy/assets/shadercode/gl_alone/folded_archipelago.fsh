precision mediump float;

// Folded Archipelago
//
// A still fractal figure forms nested islands, rounded lobes, and branching borders. Turning the controls reshapes its entire outline, splitting connected regions into smaller fragments or drawing them together into a folded central mass.
//
// par_a.x - changes the horizontal component of the fractal constant.
// par_a.y - changes the vertical component of the fractal constant.

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

precision mediump float;

const mediump float maxItr = 10.0;

void main () 
{
vec2 position = ( gl_FragCoord.xy ) / tres.xy;
vec2 c = vec2(((mouse.x) *4.0)-2.0, ((mouse.y) *9.0)-2.0);
vec2 z = vec2(((position.x) *4.0)-2.0, ((position.y) *4.0)-2.0);

    float d;
    for( float i = 0.0; i < maxItr; i += 1.0)
    {
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        
        d = dot( z, z);
        
        if( d > 4.0)
        {
            gl_FragColor = vec4( i/maxItr,i/maxItr,i/maxItr, 1.0);
            break;
        }
    }
}