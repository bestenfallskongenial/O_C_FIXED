precision mediump float;

// Sync Drift Overload
//
// Bands of bfr stretch, reverse, and slide on different rhythms. Horizontal tremors and vertical ripples bend the fractured picture further, while intermittent channel exchanges make the retained signal appear to lose its alignment repeatedly.
//
// par_a.x - horizontal displacement strength.
// par_a.y - horizontal scaling variation.
// par_a.z - vertical displacement strength.
// par_a.w - fine horizontal tremor strength.
// par_b.x - overall animation speed.
// par_b.y - number of independently distorted horizontal bands.
// par_b.z - additional vertical ripple strength.
// par_b.w - strength of intermittent channel exchanges.

uniform float time;
uniform vec2 tres;
uniform vec4 par_a;
uniform vec4 par_b;
uniform vec4 color;

uniform sampler2D bfr;

float hash(float x)
{
    return fract(
        sin(x * 12.9898)
        * 43758.5453
    );
}

void main()
{
    vec2 uv =
        gl_FragCoord.xy / tres.xy;


    float t =
        time *
        mix(
            0.5,
            12.0,
            par_b.x
        );


    float head =
        fract(
            0.5 +
            sin(t * 0.117) * 0.32 +
            sin(t * 0.371) * 0.11
        );


    float line =
        floor(
            uv.y *
            mix(
                32.0,
                240.0,
                par_b.y
            )
        );


    float linePhase =
        t * 1.71 +
        line * 0.371;

    float shift =
        sin(linePhase)
        *
        sin(t * 0.83 + line * 1.37)
        *
        mix(
            0.0,
            0.9,
            par_a.x
        );


    float stretch =
        1.0 +
        sin(
            t * 0.71 +
            line * 0.173
        )
        *
        mix(
            0.0,
            3.5,
            par_a.y
        );


    float roll =
        sin(
            t * 0.43 +
            line * 0.071
        )
        *
        mix(
            0.0,
            0.20,
            par_a.z
        );


    vec2 p =
        uv - 0.5;

    p.x *=
        stretch;

    p.x +=
        shift;

    p.y +=
        roll;


    float reverse =
        step(
            0.65,
            hash(
                line +
                floor(t * 3.0) * 17.0
            )
        );

    p.x =
        mix(
            p.x,
            -p.x,
            reverse
        );


    p.x +=
        sin(
            p.y * tres.y * 0.071 +
            t * 5.7
        )
        *
        0.025
        *
        par_a.w;


    p.y +=
        sin(
            p.x * 37.0 +
            t * 2.13
        )
        *
        0.015
        *
        par_b.z;

    vec2 srcUV =
        fract(
            p + 0.5
        );

    vec3 col =
        texture2D(
            bfr,
            srcUV
        ).rgb;


    float overload =
        step(
            0.82,
            hash(
                line * 7.0 +
                floor(t * 5.0)
            )
        );

    float channel =
        step(
            0.0,
            sin(
                t * 7.31 +
                line
            )
        );

    vec3 damaged =
        mix(
            col.brg,
            col.gbr,
            channel
        );

    col =
        mix(
            col,
            damaged,
            overload * par_b.w
        );

    gl_FragColor =
        vec4(
            clamp(
                col,
                0.0,
                1.0
            ),
            color.a
        );
}