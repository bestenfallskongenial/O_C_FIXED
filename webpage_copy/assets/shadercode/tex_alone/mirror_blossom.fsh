precision mediump float;

// Mirror Blossom
//
// Two source images fold into mirrored radial sectors around a movable center. Repeated fragments meet as petals, diamonds, and star-like seams, transforming the original pictures into a kaleidoscopic blossom.
//
// par_a.x - moves the reflection center horizontally.
// par_a.y - moves the reflection center vertically.
// par_a.z - increases the number of mirrored sectors.
// par_a.w - blends between the two source textures.

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
//f0:x offset:
//f1:y offset:
//f2:segments:
float f0 = mix(0.05, 0.95, par_a.x);
float f1 = mix(0.05, 0.95, par_a.y); 
float f2 = mix(0.05, 0.95, par_a.z);
float blend = mix(0.05, 0.95, par_a.w);

void main() 
{
    vec2 p = tcoord - vec2(f0, f1);
    float angle = atan(p.y, p.x);
    float r = length(p);
    
    angle = mod(angle, 3.14159 * 2.0 / (f2 * 12.0));
    angle = abs(angle - 3.14159 / (f2 * 12.0));
    
    p = vec2(cos(angle), sin(angle)) * r + vec2(f0, f1);
    
    vec4 col1 = texture2D(tex[0], p);
    vec4 col2 = texture2D(tex[1], p);
    gl_FragColor = vec4(mix(col1.rgb, col2.rgb, blend), color.a );
}