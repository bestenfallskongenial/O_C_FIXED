precision mediump float;

// PHANTOM EXPOSURE
//
// Bright regions of vid become luminous deposits carried through a curling flow. bfr accumulates these exposures into wandering wisps and soft trails, allowing recognizable details to dissolve into a drifting apparition.
//
// par_a.x - flow orientation; displacement grows farther from the midpoint.
// par_a.y - persistence of accumulated exposures.
// par_a.z - brightness threshold; higher values admit only brighter video regions.
// par_a.w - channel separation in the trails and spectral coloring of new deposits.

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

#define TAU 6.2831853


void main(void)
{
    vec2 uv = tcoord;

    float aspect =
        tres.x / max(tres.y, 1.0);

    vec2 p = (uv - 0.5) * 2.0;
    p.x *= aspect;


    float direction =
        (par_a.x - 0.5) * TAU;

    float ca = cos(direction);
    float sa = sin(direction);

    vec2 q = vec2(
        ca * p.x - sa * p.y,
        sa * p.x + ca * p.y
    );


    float a = sin(
        q.y * 4.7 +
        sin(q.x * 3.1) * 1.8 +
        time * 0.17
    );

    float b = cos(
        q.x * 5.3 -
        cos(q.y * 2.9) * 1.6 -
        time * 0.13
    );

    vec2 flow = vec2(
        a + b * 0.45,
        b - a * 0.45
    );


    vec2 swirl = vec2(
        -p.y,
        p.x
    );

    flow += swirl *
        sin(length(p) * 5.0 +
            time * 0.11) * 0.45;

    float strength =
        0.001 + abs(par_a.x - 0.5) * 0.045;

    vec2 displacement =
        flow * strength;

    displacement.x /= aspect;


    float separation =
        par_a.w * 0.012;

    vec2 drift = normalize(
        flow + vec2(0.0001)
    ) * separation;

    drift.x /= aspect;

    vec2 baseUV =
        uv - displacement;

    vec3 history;

    history.r = texture2D(
        bfr,
        clamp(baseUV + drift, 0.0, 1.0)
    ).r;

    history.g = texture2D(
        bfr,
        clamp(baseUV, 0.0, 1.0)
    ).g;

    history.b = texture2D(
        bfr,
        clamp(baseUV - drift, 0.0, 1.0)
    ).b;


    vec3 video = texture2D(
        vid, uv
    ).rgb;

    float luminance = dot(
        video,
        vec3(0.299, 0.587, 0.114)
    );


    float threshold = mix(
        0.08,
        0.85,
        par_a.z
    );

    float extraction = smoothstep(
        threshold,
        threshold + 0.12,
        luminance
    );


    float detail = abs(
        sin(
            p.x * 19.0 +
            p.y * 11.0 +
            a * 4.0
        )
    );

    extraction *=
        0.65 + 0.35 * detail;


    vec3 spectrum =
        0.5 + 0.5 * cos(
            TAU * (
                luminance * 0.7 +
                a * 0.12 +
                vec3(0.0, 0.33, 0.67)
            )
        );

    vec3 source = mix(
        vec3(luminance),
        spectrum * luminance,
        par_a.w
    );

    source *= extraction;


    float decay = mix(
        0.70,
        0.995,
        par_a.y
    );

    vec3 accumulated =
        history * decay;

    accumulated +=
        source * 0.12;


    vec3 result =
        1.0 - exp(-accumulated * 1.6);


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
