precision mediump float;

// CHRYSALIS
//
// A rounded membrane carries rippling folds, bright creases, and moving surface reflections. Its slowly turning relief gathers around a central region, while a subtly scalloped boundary gives the form the appearance of a soft, enclosed shell.
//
// par_a.x - fold shaping, crease emphasis, and boundary undulation.
// par_a.y - surface ripple density.
// par_a.z - surface orientation and size of the central region with reduced height-based coloring.
// par_a.w - material hue offset.

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

#define TAU 6.28318530718


float surface(vec2 p, float freq,
              float fold, float phase)
{
    float a = sin(
        p.x * freq +
        sin(p.y * freq * 0.7 + phase)
        * (1.0 + fold * 3.0)
    );

    float b = cos(
        p.y * freq * 0.83 -
        sin(p.x * freq * 0.6 - phase)
        * (1.0 + fold * 2.5)
    );

    float c = sin(
        length(p) * freq * 1.6 -
        phase
    );

    return
        a * 0.45 +
        b * 0.35 +
        c * 0.2;
}

void main(void)
{
    vec2 uv = tcoord;

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / max(tres.y, 1.0);

    float fold = par_a.x;
    float freq = mix(
        2.0, 11.0, par_a.y
    );

    float opening = par_a.z;
    float phase = time * 0.16;


    vec2 q = p;

    float twist =
        opening * 2.4 +
        phase * 0.2;

    float ct = cos(twist);
    float st = sin(twist);

    q = vec2(
        q.x * ct - q.y * st,
        q.x * st + q.y * ct
    );


    float h = surface(
        q, freq, fold, phase
    );


    float crease = abs(h);

    float folded = mix(
        h,
        sign(h) * pow(
            crease,
            0.35 + fold * 1.5
        ),
        fold
    );


    float radius = length(q);

    float aperture = smoothstep(
        0.12 + opening * 0.5,
        0.18 + opening * 0.5,
        radius
    );

    float height =
        folded * aperture;


    float e = 0.004;

    float hx = surface(
        q + vec2(e, 0.0),
        freq, fold, phase
    );

    float hy = surface(
        q + vec2(0.0, e),
        freq, fold, phase
    );

    vec3 normal = normalize(
        vec3(
            (h - hx) / e,
            (h - hy) / e,
            1.0
        )
    );


    vec3 lightA = normalize(
        vec3(-0.6, 0.8, 1.2)
    );

    vec3 lightB = normalize(
        vec3(0.8, -0.4, 0.6)
    );

    float diffuseA = max(
        dot(normal, lightA),
        0.0
    );

    float diffuseB = max(
        dot(normal, lightB),
        0.0
    );


    float hue =
        height * 0.22 +
        radius * 0.17 +
        par_a.w;

    vec3 material = 0.5 + 0.5 * cos(
        TAU * (
            hue +
            vec3(0.0, 0.31, 0.67)
        )
    );


    vec3 viewDir = vec3(
        0.0, 0.0, 1.0
    );

    vec3 halfVector = normalize(
        lightA + viewDir
    );

    float specular = pow(
        max(
            dot(normal, halfVector),
            0.0
        ),
        32.0
    );


    float ridge = exp(
        -abs(h) * (12.0 + fold * 25.0)
    );


    float boundary =
        0.88 +
        sin(
            atan(q.y, q.x) * 7.0 +
            phase
        ) * fold * 0.08;

    float mask = 1.0 - smoothstep(
        boundary - 0.035,
        boundary + 0.015,
        radius
    );


    vec3 result =
        material *
        (0.12 +
         diffuseA * 0.75 +
         diffuseB * 0.3);

    result += specular *
        vec3(0.9, 0.95, 1.0) *
        0.8;

    result += ridge *
        vec3(0.3, 0.65, 0.85) *
        fold * 0.4;


    vec3 background = vec3(
        0.006,
        0.009,
        0.018
    );

    result = mix(
        background,
        result,
        mask
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
