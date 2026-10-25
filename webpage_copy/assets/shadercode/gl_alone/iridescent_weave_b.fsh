precision mediump float;

// Wave Brocade
//
// Crossing waves weave into chevrons, loops, and tangled meshes. The arrangement twists between orderly rows and densely knotted intersections, like patterned fabric stretched and released in several directions.
//
// par_a.x - reduces the cosine-driven contribution to the wave transformation.
// par_a.y - reduces the sine-driven contribution to the wave transformation.
// par_a.z - increases pattern density, making the weave finer.

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
     #define M_PI 3.141592653589793

void main()
{
    float f0 = mix(0.05, 0.95, par_a[0]);  // wave 1
    float f1 = mix(0.05, 0.95, par_a[1]);  // wave 2
    float f2 = mix(0.05, 0.95, par_a[2]);  // zoom

    vec2 position = (25.0 * f2 + 5.0) * (tcoord * tres * 2.0 - tres) / tres.x;

    float a = 0.1 *  time * M_PI / 3.0;
    float c = cos(a) / f0;
    float s = sin(a) / f1;
    mat2 rotate = mat2(c, s, -s, c);
    vec2 q = 1.0 + sin(position + M_PI * sin( time - position.yx));
    vec2 q2 = 1.0 + sin(1.2 * position + M_PI * sin( time - 1.4 * position.yx));
    q = sqrt(abs(q * rotate));

    vec3 color_out = sqrt(normalize(vec3(q.x - q2.y, reflect(q.x, q.y) + q2.y, q.y + reflect(q2.x, q2.y))));

    gl_FragColor = vec4(color_out, color.a );
}