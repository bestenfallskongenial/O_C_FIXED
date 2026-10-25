precision mediump float;

// Revolving Wells
//
// Two orbiting centers pull the surrounding field into lens-like regions and sweeping seams. Fine bands gather near their cores while broader shapes stretch between them, opening and narrowing as the centers revolve.
//
// par_a.x - increases horizontal coordinate scale.
// par_a.y - increases vertical coordinate scale.
// par_a.z - reduces the second inversion center’s contribution.

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
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:point zoom:

void main()
{
    vec2 p = (8.0 * (gl_FragCoord.xy / tres.xy) - 4.0) * vec2(f0, f1);
    p.x *= tres.x / tres.y;
    
    vec2 c = 2.0 * vec2(cos(time * 0.2), sin(time * 0.2));
    vec2 d = 2.0 * vec2(cos(time * 0.2 + 3.14), sin(time * 0.2 + 3.14));

    // Mix inversions
    p = (p - c) / dot(p - c, p - c) - (p - d) / dot(p - d, p - d) / f2;

    // RGB
    float r = mod(exp(p.y), exp(p.y / p.x));
    float g = mod(exp(p.x), exp(dot(p, c * p)));
    float b = mod(exp(p.x) * exp(p.y), exp(dot(p, d * p)));

    gl_FragColor = vec4 ( 0.5 * clamp(abs(vec3(r, g, b)), 0.0, 2.0) , color.a);

//  vec3 color_out = 0.5 * clamp(abs(vec3(r, g, b)), 0.0, 2.0);
//  gl_FragColor = vec4(color_out, color.a);
}