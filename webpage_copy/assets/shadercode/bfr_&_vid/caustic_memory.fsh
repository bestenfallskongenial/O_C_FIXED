precision mediump float;

// CAUSTIC MEMORY
//
// Interacting waves bend the image into flowing folds, gathering light along thin, shifting seams. vid supplies the refracted picture, while bfr preserves drifting traces of the luminous network.
//
// par_a.x - refraction strength and intensity of the folds.
// par_a.y - wave density.
// par_a.z - blends the generated illumination toward the refracted video.
// par_a.w - strength and persistence of the backbuffer afterimages.

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


// Analytic displacement and its
// local spatial derivatives.
//
// The derivatives let us estimate
// light concentration without
// sampling neighboring pixels.

void main(void)
{
    vec2 uv = tcoord;

    float aspect =
        tres.x / max(tres.y, 1.0);

    vec2 p = (uv - 0.5) * 2.0;
    p.x *= aspect;

    float scale =
        mix(2.5, 12.0, par_a.y);

    float strength =
        mix(0.025, 0.28, par_a.x);

    float t = time * 0.17;


    float a =
        p.x * scale * 0.83 +
        p.y * scale * 0.31 +
        t;

    float b =
        p.x * scale * -0.37 +
        p.y * scale * 0.91 -
        t * 0.73;

    float c =
        p.x * scale * 0.56 -
        p.y * scale * 0.72 +
        t * 0.41;

    float sa = sin(a);
    float sb = sin(b);
    float sc = sin(c);

    float ca = cos(a);
    float cb = cos(b);
    float cc = cos(c);


    vec2 warp = strength * vec2(
        sa + 0.65 * sb - 0.4 * sc,
        0.55 * sa - sb + 0.7 * sc
    );


    float ax = scale * 0.83;
    float ay = scale * 0.31;

    float bx = scale * -0.37;
    float by = scale * 0.91;

    float cx = scale * 0.56;
    float cy = scale * -0.72;

    float jxx = 1.0 + strength * (
        ca * ax +
        0.65 * cb * bx -
        0.4 * cc * cx
    );

    float jxy = strength * (
        ca * ay +
        0.65 * cb * by -
        0.4 * cc * cy
    );

    float jyx = strength * (
        0.55 * ca * ax -
        cb * bx +
        0.7 * cc * cx
    );

    float jyy = 1.0 + strength * (
        0.55 * ca * ay -
        cb * by +
        0.7 * cc * cy
    );

    float determinant =
        jxx * jyy - jxy * jyx;


    float focus = 1.0 /
        (0.12 + abs(determin));

    focus = min(focus, 6.0);

    float filament = exp(
        -abs(determin) * 9.0
    );


    vec2 sampleUV =
        uv + vec2(
            warp.x / aspect,
            warp.y
        ) * 0.23;

    vec3 video = texture2D(
        vid,
        clamp(sampleUV, 0.0, 1.0)
    ).rgb;


    float spectrumPhase =
        a * 0.21 +
        b * 0.17 +
        c * 0.13;

    vec3 spectral = 0.5 + 0.5 * cos(
        spectrumPhase +
        vec3(0.0, 2.1, 4.2)
    );


    vec3 light = mix(
        vec3(0.12, 0.55, 0.85),
        spectral,
        0.75
    );

    vec3 pigment = mix(
        light,
        video,
        par_a.z
    );

    vec3 current =
        pigment * focus * 0.22;

    current += filament *
        vec3(0.75, 0.9, 1.0) *
        0.7;


    current += vec3(
        0.008,
        0.015,
        0.025
    );


    vec2 historyUV = uv -
        vec2(
            warp.x / aspect,
            warp.y
        ) * 0.012;

    vec3 history = texture2D(
        bfr,
        clamp(historyUV, 0.0, 1.0)
    ).rgb;

    float persistence = mix(
        0.15,
        0.985,
        par_a.w
    );

    vec3 accumulated = max(
        current,
        history * persistence
    );

    vec3 result = mix(
        current,
        accumulated,
        par_a.w
    );


    result = result /
        (1.0 + result * 0.35);

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
