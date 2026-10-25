precision mediump float;

// CHROMATIC STRATA
//
// Nested bands wind through a deeply folded field, forming flowing strata, narrow seams, and enclosed pockets. Fine filaments trace the layers like polished mineral boundaries in a still, intricately contoured surface.
//
// par_a.x - strength and complexity of the coordinate folding.
// par_a.y - layer density.
// par_a.z - shifts the layer positions and palette.
// par_a.w - boundary darkness, filament brightness, and overall contrast.

varying vec2 tcoord;

uniform float time;
uniform vec2 tres;

uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;

uniform sampler2D tex[8];
uniform int tex_l;

uniform sampler2D bfr;
uniform sampler2D vid;

#define PI 3.14159265359


vec3 palette(float x)
{
    return 0.5 + 0.5 * cos(
        6.2831853 *
        (vec3(0.00, 0.33, 0.67) + x)
    );
}

void main(void)
{
    vec2 p = tcoord * 2.0 - 1.0;

    p.x *= tres.x / max(tres.y, 1.0);

    float fold = par_a.x;
    float density = par_a.y;
    float phase = par_a.z;
    float contrast = par_a.w;


    vec2 q = p;

    q.x += sin(
        q.y * (3.0 + fold * 9.0)
        + sin(q.x * 2.7)
    ) * (0.12 + fold * 0.42);

    q.y += cos(
        q.x * (4.0 + fold * 7.0)
        - sin(q.y * 3.1)
    ) * (0.10 + fold * 0.36);


    vec2 r = q;

    r.x += 0.27 * sin(
        q.y * 7.0 + q.x * 4.0
    );

    r.y += 0.23 * cos(
        q.x * 6.0 - q.y * 5.0
    );


    float field =
        r.x * 2.3 +
        r.y * 1.7;

    field += 0.85 * sin(
        r.x * 3.5 +
        r.y * 2.4
    );

    field += 0.55 * cos(
        r.y * 5.8 -
        r.x * 2.9
    );

    field += 0.3 * sin(
        length(r) * 12.0
    );


    float layers =
        mix(4.0, 24.0, density);

    float coordinate =
        field * layers * 0.25 + phase * 8.0;

    float band = floor(coordinate);
    float local = fract(coordinate);


    vec3 base = palette(
        band * 0.071 + phase
    );

    vec3 secondary = palette(
        band * 0.071 + 0.17 + phase
    );

    float ribbon = smoothstep(
        0.08, 0.55, local
    );

    vec3 result = mix(
        base,
        secondary,
        ribbon * 0.65
    );


    float edge = 1.0 - smoothstep(
        0.0,
        0.035,
        min(local, 1.0 - local)
    );

    result *= 1.0 -
        edge * (0.3 + contrast * 0.7);


    float filament = 1.0 - smoothstep(
        0.005,
        0.035,
        abs(local - 0.48)
    );

    result += vec3(
        0.8, 0.9, 1.0
    ) * filament * contrast * 0.6;


    result = mix(
        result,
        smoothstep(
            vec3(0.25),
            vec3(0.8),
            result
        ),
        contrast * 0.65
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
