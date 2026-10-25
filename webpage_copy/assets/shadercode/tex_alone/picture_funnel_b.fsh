precision mediump float;

// Reverse Picture Funnel
//
// An external image wraps into an inverted perspective arrangement of curved, receding tiles. A reversed checker mask reveals the complementary cells, exchanging the picture-filled regions and dark gaps around the central opening.
//
// par_a.x - changes the strength of the radial inversion.
// par_a.y - shifts the perspective horizon.

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

void main( void )
{
    vec2 pos = ( gl_FragCoord.xy / tres.xy ) * 2.0 - 1.0;
    pos = par_a.x*pos / dot(pos,pos);
    pos *= cos(pos);
    
    float horizon = par_a.y;
    float fov = 0.85; 
    float scaling = 0.1;
    
    vec3 p = vec3(pos.x, fov, pos.y - horizon);      
    vec2 s = vec2((p.x)/p.z, p.y/p.z) * scaling;
    
    // Original checkboard but inverted
    float mask = -sign((mod(s.x, 0.1) - 0.05) * (mod(s.y, 0.1) - 0.05));
    
    vec2 texCoord = (s + 1.0) * 0.5;
    vec4 texColor = texture2D(tex[0], texCoord);
    
    vec3 color_out = (mask > 0.0) ? texColor.rgb : vec3(0.0);
    color_out *= p.z*p.z*10.0;
    
    gl_FragColor = vec4(color_out, color.a );
}