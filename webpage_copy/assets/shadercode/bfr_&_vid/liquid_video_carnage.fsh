precision mediump float;

// LIQUID VIDEO CARNAGE
//
// vid twists into repeated folds and broken mirrored fragments. bfr steers the deformation and feeds distorted imagery back into itself, making the picture buckle, churn, and occasionally turn inside out.
//
// par_a.x - strength of twisting, radial distortion, and folding.
// par_a.y - density of folds and repetitions.
// par_a.z - backbuffer displacement and feedback blend.
// par_a.w - animation speed, unstable rotation, and inversion strength.

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


mat2 rot(float a)
{
    float c = cos(a);
    float s = sin(a);
    return mat2(c, -s, s, c);
}

void main(void)
{
    vec2 uv = tcoord;
    vec2 p = uv * 2.0 - 1.0;

    p.x *= tres.x / tres.y;

    float violence = par_a.x;
    float folds    = par_a.y;
    float feedback = par_a.z;
    float chaos    = par_a.w;


    vec3 old = texture2D(bfr, uv).rgb;

    float energy = dot(
        old,
        vec3(0.299, 0.587, 0.114)
    );


    float radius = length(p);
    float angle = atan(p.y, p.x);

    float phase =
        time * (0.1 + chaos * 5.0);

    float twist =
        sin(
            radius * mix(3.0, 85.0, folds)
            - phase
            + energy * 12.0
        );

    angle += twist * violence * 5.0;


    float wave =
        sin(
            angle * mix(2.0, 28.0, folds)
            + radius * 19.0
            - phase * 2.0
        );

    radius +=
        wave * violence * 0.35;


    vec2 q = vec2(
        cos(angle),
        sin(angle)
    ) * radius;

    q = abs(
        q * mix(1.0, 7.0, folds)
    );

    q = abs(fract(q * 0.5) * 2.0 - 1.0);

    q = mix(
        p,
        q - 0.5,
        violence
    );


    vec2 displacement =
        (old.rg * 2.0 - 1.0)
        * feedback
        * 0.45;

    q += displacement;


    float unstable =
        sin(
            time * 7.0
            + energy * 23.0
            + q.y * 15.0
        );

    q *= rot(
        unstable * chaos * 2.5
    );


    vec2 sourceUV =
        fract(q * 0.5 + 0.5);

    vec3 video =
        texture2D(vid, sourceUV).rgb;


    vec2 feedbackUV = fract(
        sourceUV
        + displacement
        + vec2(
            sin(phase + energy * 8.0),
            cos(phase - energy * 9.0)
        ) * feedback * 0.08
    );

    vec3 previous =
        texture2D(bfr, feedbackUV).rgb;


    float blend =
        feedback
        * (0.35 + energy * 0.6);

    vec3 result =
        mix(video, previous, blend);


    float inversion =
        step(
            0.0,
            sin(
                angle * (3.0 + folds * 30.0)
                + phase
            )
        );

    result = mix(
        result,
        1.0 - result.bgr,
        inversion * violence * chaos
    );

    gl_FragColor =
        vec4(clamp(result, 0.0, 1.0), color.a);
}
