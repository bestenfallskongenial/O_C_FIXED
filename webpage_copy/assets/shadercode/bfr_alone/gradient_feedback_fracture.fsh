precision mediump float;

// Gradient Feedback Fracture
//
// Edges in bfr become a pulsing transport field. Rotating samples and amplified local differences fracture the retained picture into unstable seams and granular regions, while repeated tonal wrapping and channel cycles continually disturb its surface.
//
// par_a.x - strength of edge-driven displacement.
// par_a.y - rotation speed of the displacement field.
// par_a.z - amplification of local differences.
// par_a.w - tonal gain before values wrap.
// par_b.x - overall animation speed.
// par_b.y - rotation speed of the neighboring sampling directions.
// par_b.z - channel-cycling rate.
// par_b.w - strength of the small oscillating signal added each frame.

uniform float time;
uniform vec2 tres;
uniform vec4 par_a;
uniform vec4 par_b;
uniform vec4 color;

uniform sampler2D bfr;

void main()
{
    vec2 uv =
        gl_FragCoord.xy / tres.xy;

    vec2 px =
        1.0 / tres.xy;


    float t =
        time *
        mix(
            0.5,
            10.0,
            par_b.x
        );


    float a =
        t *
        mix(
            0.1,
            3.0,
            par_b.y
        );

    float cs =
        cos(a);

    float sn =
        sin(a);

    vec2 dx =
        vec2(
            cs,
            sn
        )
        *
        px;

    vec2 dy =
        vec2(
            -sn,
             cs
        )
        *
        px;


    vec3 c =
        texture2D(
            bfr,
            uv
        ).rgb;

    vec3 l =
        texture2D(
            bfr,
            fract(uv - dx)
        ).rgb;

    vec3 r =
        texture2D(
            bfr,
            fract(uv + dx)
        ).rgb;

    vec3 d =
        texture2D(
            bfr,
            fract(uv - dy)
        ).rgb;

    vec3 u =
        texture2D(
            bfr,
            fract(uv + dy)
        ).rgb;


    vec2 gradient;

    gradient.x =
        dot(
            r - l,
            vec3(0.333)
        );

    gradient.y =
        dot(
            u - d,
            vec3(0.333)
        );


    float pulse =
        sin(t * 1.73);

    float gain =
        mix(
            1.0,
            90.0,
            par_a.x
        )
        *
        pulse;

    gradient *=
        gain;


    float rot =
        t *
        mix(
            0.0,
            6.28318,
            par_a.y
        );

    float rc =
        cos(rot);

    float rs =
        sin(rot);

    gradient =
        mat2(
             rc, -rs,
             rs,  rc
        )
        *
        gradient;


    gradient *=
        sin(
            t * 2.17 +
            uv.y * 17.0
        );

    vec2 fuv =
        fract(
            uv +
            gradient * px
        );

    vec3 history =
        texture2D(
            bfr,
            fuv
        ).rgb;


    vec3 prediction =
        (
            l +
            r +
            u +
            d
        )
        *
        0.25;

    vec3 error =
        c -
        prediction;


    float errorGain =
        mix(
            0.0,
            12.0,
            par_a.z
        )
        *
        (
            0.5 +
            0.5 *
            sin(t * 3.11)
        );

    history +=
        error *
        errorGain;


    float drive =
        mix(
            1.0,
            8.0,
            par_a.w
        );

    drive *=
        0.65 +
        0.35 *
        sin(
            t * 1.37
        );

    vec3 col =
        fract(
            abs(history) *
            drive
        );


    float channelPhase =
        fract(
            t *
            mix(
                0.1,
                2.0,
                par_b.z
            )
        );

    if (channelPhase > 0.666)
    {
        col =
            col.gbr;
    }
    else if (channelPhase > 0.333)
    {
        col =
            col.brg;
    }


    col +=
        sin(
            t *
            vec3(
                1.71,
                2.13,
                2.79
            )
        )
        *
        0.006
        *
        par_b.w;

    gl_FragColor =
        vec4(
            fract(col),
            color.a
        );
}