precision mediump float;

// Breathing Tiles
//
// A regular grid of softly rounded squares pulses with independently shifting phases. Tiles swell toward their boundaries, shrink into smaller patches, and fade away, opening temporary gaps in the breathing mosaic.
//
// par_a.x - increases the number of square cells across the image.
// par_a.y - changes how fully the squares fill their cells and how often they disappear.
// par_a.z - raises the palette’s brightness baseline.

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

//f0:scale:
//f1:squares:
//f2:color_out pos:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

void main()
{
    vec2 px = (1.0 + 8.0 * f0) * (-tres.xy + 2.0 * gl_FragCoord.xy) / tres.y;
    
    float id = f1 + 0.5 * cos(time + sin(dot(floor(px + 0.5), vec2(113.1, 17.81))) * 43758.545);
    
    vec3 co = f2 + 0.5 * cos(time + 3.5 * id + vec3(0.0, 1.57, 3.14));
    
    vec2 pa = smoothstep(0.0, 0.2, id * (0.5 + 0.5 * cos(6.2831 * px)));
    
    vec3 color_out = co * pa.x * pa.y;
    gl_FragColor = vec4(color_out, color_out.a);
}