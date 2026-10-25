precision mediump float;

// Fractal Anemone
//
// Branching tendrils curl around smooth chambers and frilled borders. The evolving fractal repeatedly opens its folds and gathers them into tighter spirals, like an anemone turning and unfurling beneath a current.
//
// par_a.x - increases coordinate scale, revealing a wider portion of the fractal.
// par_a.y - rotates the fractal through a full turn.
// par_a.z - blends toward inverted colors; halfway removes the pattern’s contrast.

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
 
// Complex number operations
vec2 cmul(vec2 a, vec2 b)
{
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

vec2 cdiv(vec2 a, vec2 b)
{
    float denom = dot(b, b);
    return vec2(dot(a, b) / denom, (a.y * b.x - a.x * b.y) / denom);
}

vec2 cexp(vec2 z)
{
    return exp(z.x) * vec2(cos(z.y), sin(z.y));
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
        float zoom = 0.5 + par_a[0] * 1.5; // Scale factor
        float rotation = par_a[1] * 360.0; // Rotation angle

        uv *= zoom;
    float angle = radians(rotation);
    uv = vec2(
        uv.x * cos(angle) - uv.y * sin(angle),
        uv.x * sin(angle) + uv.y * cos(angle)
    );
        // Hyperbolic transformation
    vec2 z = cdiv(cexp(uv), vec2(1.0, 0.0) + cexp(uv));
        // Iterate to create fractal pattern
    vec2 c = vec2(0.7885 * cos(time), 0.7885 * sin(time));
    float iterations = 0.0;
    for (int i = 0; i < 64; i++)
        {
        z = cmul(z, z) + c;
        if (length(z) > 2.0) break;
        iterations += 1.0;
        }
        // Coloring
    vec3 color_out = vec3(0.0);
    color_out.r = 0.5 + 0.5 * sin(0.1 * iterations + (time*0.001));
    color_out.g = 0.5 + 0.5 * sin(0.1 * iterations + (time*0.001) + 2.094);
    color_out.b = 0.5 + 0.5 * sin(0.1 * iterations + (time*0.001)    + 4.188);
        // Add some variation
    color_out *= 1.0 - length(z) / 2.0;
    color_out += 0.1 * sin(10.0 * z.xyx + time);
        // Apply par_a[0] for color_out variation
    color_out = mix(color_out, 1.0 - color_out, par_a[2]);
        gl_FragColor = vec4(color_out, color.a);
}