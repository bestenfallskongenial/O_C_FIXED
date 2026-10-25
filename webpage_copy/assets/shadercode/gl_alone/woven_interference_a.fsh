precision mediump float;

// Woven Interference
//
// Crossing wave lattices slide through one another, opening soft pockets and gathering into denser knots. A slower interference layer bends the arrangement, making the surface breathe between broad undulations and a tightly woven mesh.
//
// par_a.x - sets horizontal frequency in the additional interference layer.
// par_a.y - sets vertical frequency in that layer.
// par_a.z - increases coordinate scale, making the pattern finer.
// par_a.w - rotates the pattern through a full turn.
// par_b.x - reduces saturation.

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

float zoom = mix(0.0, 0.5, par_a[2]);
float rotation = mix(0.0,360.0,par_a[3]);

float f0 = mix(0.1, 0.95, par_a[0]);    // f0: :
float f1 = mix(0.05, 0.95, par_a[1]);   // f1: :
float f2 = mix(0.05, 0.95, par_a[2]);   // f2: :

void main()
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
    uv *= zoom;
    float angle = radians(rotation);
    uv = vec2( uv.x * cos(angle) - uv.y * sin(angle), uv.x * sin(angle) + uv.y * cos(angle) );
    // Create interference patterns
    float pattern1 = sin(uv.x * 50.0 +  time * 2.0) * sin(uv.y * 50.0 +  time * 3.0);
    float pattern2 = sin((uv.x + uv.y) * 40.0 -  time * 4.0) * sin((uv.x - uv.y) * 40.0 +  time * 5.0);
    // Combine patterns with varying frequencies
    float combinedPattern = pattern1 * 0.5 + pattern2 * 0.5;
    combinedPattern += sin(uv.x * 30.0 * par_a[0] +  time) * sin(uv.y * 30.0 * par_a[1] -  time) * 0.25;
    // Create holographic color_out effect
    vec3 color_out = vec3(0.5) + 0.5 * cos( time + uv.xyx + vec3(0, 2, 4));
    // Apply interference pattern to color_out
    color_out *= 0.5 + 0.5 * combinedPattern;
    // Add some depth and contrast
    color_out = mix(color_out, vec3(dot(color_out, vec3(0.299, 0.587, 0.114))), par_b[0]);
    color_out = pow(color_out, vec3(0.8 + 0.4 * sin( time * 0.5)));

    gl_FragColor = vec4(color_out, color.a);
}