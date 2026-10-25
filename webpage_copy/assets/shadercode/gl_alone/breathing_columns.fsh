precision mediump float;

// Breathing Columns
//
// Tall repeating bands are interrupted by rounded patches that swell and recede. The crossing waves open holes, stretch their edges, and close them again, like a row of columns breathing through a flexible screen.
//
// par_a.x - increases horizontal repetition, making the columns narrower.
// par_a.y - increases vertical repetition within the crossing patches.
// par_a.z - reduces the strength of the patches crossing the vertical bands.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:horz stretch:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert stretch:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:bubble ups:

void main()
{
    vec2 position = (gl_FragCoord.xy / tres.xy) * vec2(f0, f1);
    vec3 couleur = vec3(4.0, 3.5, 3.0) / cos(position.x * 20.0) * tan(position.x * 40.0);
    vec3 couleur2 = vec3(12.0, 12.0, 12.0) * sin(position.y * 10.0) * cos(position.x * 10.0);
    vec3 couleur3 = vec3(1.4, 2.0, 0.0) * sin(position.y * 10.0) * sin(position.x * 15.0);
    vec3 finalColor = couleur + couleur3 / f2 + couleur2 / f2 * sin(time * 2.0);

    gl_FragColor = vec4(finalColor, color.a);
}