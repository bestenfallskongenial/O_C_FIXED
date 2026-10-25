precision mediump float;

// Turning Picturewell
//
// An inverted checkerboard gathers into nested curved cells around a small opening. Source images can turn beneath the checker mask, changing the fragments visible through its fixed divisions without rotating the surrounding geometry.
//
// par_a.x - changes the strength of the radial inversion.
// par_a.y - shifts the perspective horizon.
// par_a.z - selects plain checkerboard, either single-image mask, or two interleaved images in four equal ranges.
// par_a.w - rotates the source images through a full turn.

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
    
    // Convert to UV space and apply rotation
    vec2 texCoord = (s + 1.0) * 0.5;  // Convert to 0-1 range
    
    // Apply rotation to tex coordinates
    float angle = par_a.w * 6.28318; // 0-1 to 0-2PI
    vec2 center = vec2(0.5, 0.5);
    texCoord -= center;  // Move to origin
    texCoord = vec2(
        texCoord.x * cos(angle) - texCoord.y * sin(angle),
        texCoord.x * sin(angle) + texCoord.y * cos(angle)
    );
    texCoord += center;  // Move back
    
    // Basic mask for checkboard
    float mask = sign((mod(s.x, 0.1) - 0.05) * (mod(s.y, 0.1) - 0.05));
    
    vec3 color_out;
    
    if(par_a.z < 0.25)
{
        // Mode 1: Original B&W checkboard
        color_out = vec3(mask > 0.0 ? 1.0 : 0.0);
    }
    else if(par_a.z < 0.5)
{
        // Mode 2: Single tex on white areas with rotation
        vec4 texColor = texture2D(tex[0], texCoord);
        color_out = (mask > 0.0) ? texColor.rgb : vec3(0.0);
    }
    else if(par_a.z < 0.75)
{
        // Mode 3: Inverted B&W with tex and rotation
        vec4 texColor = texture2D(tex[0], texCoord);
        color_out = (mask < 0.0) ? texColor.rgb : vec3(0.0);
    }
    else
{
        // Mode 4: Two textures with rotation
        vec4 tex0Color = texture2D(tex[0], texCoord);
        vec4 tex1Color = texture2D(tex[1], texCoord);
        color_out = (mask > 0.0) ? tex0Color.rgb : tex1Color.rgb;
    }
    
    color_out *= p.z*p.z*10.0;
    
    gl_FragColor = vec4(color_out, color.a );
}