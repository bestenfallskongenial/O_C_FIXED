precision mediump float;

// Branching Shores
//
// Fractal boundaries curl around broad smooth regions, gathering into fine spirals and branching filaments. The evolving figure opens and closes its inlets, like a shoreline continually reshaped into more intricate folds.
//
// par_a.x - increases coordinate scale, revealing a wider portion of the fractal.
// par_a.y - increases the reach of the animated fractal deformation.
// par_a.z - raises the escape threshold, changing boundaries and interior coverage.

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

float round(float a)
{
    if(a < 0.5)
        return 0.;
    return 1.;
}

vec3 frctl(vec2 a)
{
    a = a * 8.;
    a = vec2(a.x - 3.0, a.y - 1.5);

    vec2 c = vec2(sin(time * 0.5) * 1.2, cos(time * 0.5) * 3.0) * par_a[1];
    vec2 z = a * par_a[0];

    float r = 1.0, g = 1.0, b = 1.0;

    for(float i = 0.0; i < 8.0; i++) 
        {
        z = vec2(z.x * z.x - z.y * z.y, z.y * z.x + z.x * z.y);
        z = vec2(z.x + c.x, z.y + c.y);

        z = vec2(z.x * z.x - z.y * z.y, z.y * z.x + z.x * z.y);
        z = vec2(z.x + c.x, z.y + c.y);

        z = vec2(z.x * z.x - z.y * z.y, z.y * z.x + z.x * z.y);
        z = vec2(z.x + c.x, z.y + c.y);

        z = vec2(z.x * z.x - z.y * z.y, z.y * z.x + z.x * z.y);
        z = vec2(z.x + c.x, z.y + c.y);

        if(sqrt(z.x * z.x + z.y * z.y) > 1900.0 * par_a[2])
            {
            r = i / 256.0;
            g = z.x;
            b = z.y;
            break;
            }
        }
    r = pow(r, 10.0);

    return vec3(r, g, b);
}

void main()
{
    vec2 position = (gl_FragCoord.xy / (tres * 2.0));
    vec3 colorout = frctl(position);

    gl_FragColor = vec4(colorout, color.a);
}