precision mediump float;

// Bitplane Fracture
//
// Separate channels of bfr break into independently rearranged grids. Fragments repeat within shifting groups, become coarsely sampled, and pass through changing tonal steps. Scattered regions collapse into hard thresholds or lose a channel, making the retained picture resemble a damaged digital mosaic.
//
// par_a.x - overall animation and disruption speed.
// par_a.y - horizontal grid density.
// par_a.z - vertical grid density.
// par_a.w - horizontal size of fragment-copying groups.
// par_b.x - vertical size of fragment-copying groups.
// par_b.y - source-fragment selection rate.
// par_b.z - horizontal sampling subdivisions inside fragments.
// par_b.w - vertical sampling subdivisions inside fragments.

uniform float time;
uniform vec2 tres;

uniform vec4 par_a;
uniform vec4 par_b;
uniform vec4 color;

uniform sampler2D bfr;


float hash21(vec2 p)
{
    return fract(
        sin(dot(p, vec2(127.1, 311.7)))
        * 43758.5453
    );
}


vec2 fractureMap(
    vec2 uv,
    float seed
)
{
    float t =
        time *
        mix(0.25, 12.0, par_a.x);



    vec2 grid;

    grid.x =
        24.0 +
        floor(
            (
                0.5 +
                0.5 *
                sin(
                    t * 0.83 +
                    seed
                )
            )
            *
            mix(16.0, 180.0, par_a.y)
        );


    grid.y =
        12.0 +
        floor(
            (
                0.5 +
                0.5 *
                cos(
                    t * 0.61 +
                    seed * 1.73
                )
            )
            *
            mix(10.0, 100.0, par_a.z)
        );


    vec2 p =
        uv * grid;

    vec2 cell =
        floor(p);

    vec2 local =
        fract(p);



    vec2 span;

    span.x =
        2.0 +
        floor(
            mix(1.0, 18.0, par_a.w)
            *
            (
                0.3 +
                0.7 *
                abs(
                    sin(
                        t * 1.37 +
                        seed * 3.7
                    )
                )
            )
        );


    span.y =
        2.0 +
        floor(
            mix(1.0, 12.0, par_b.x)
            *
            (
                0.3 +
                0.7 *
                abs(
                    cos(
                        t * 1.91 +
                        seed * 5.1
                    )
                )
            )
        );


    vec2 group =
        floor(
            cell / span
        );


    vec2 start =
        group * span;


    vec2 end =
        min(
            start + span,
            grid
        );


    vec2 available =
        max(
            end - start,
            vec2(1.0)
        );



    float tick =
        floor(
            t *
            mix(3.0, 41.0, par_b.y)
        );


    vec2 rnd =
        vec2(
            hash21(
                group +
                vec2(
                    seed * 17.0,
                    tick
                )
            ),

            hash21(
                group.yx +
                vec2(
                    tick * 1.713,
                    seed * 31.0
                )
            )
        );


    vec2 source =
        start +
        floor(
            rnd * available
        );



    vec2 quant;

    quant.x =
        1.0 +
        floor(
            mix(1.0, 16.0, par_b.z)
            *
            (
                0.25 +
                0.75 *
                abs(
                    sin(
                        t * 2.71 +
                        seed
                    )
                )
            )
        );


    quant.y =
        1.0 +
        floor(
            mix(1.0, 12.0, par_b.w)
            *
            (
                0.25 +
                0.75 *
                abs(
                    cos(
                        t * 3.17 +
                        seed
                    )
                )
            )
        );


    local =
        (
            floor(local * quant)
            +
            0.5
        )
        /
        quant;


    return
        (
            source +
            local
        )
        /
        grid;
}


void main()
{
    vec2 uv =
        gl_FragCoord.xy /
        tres.xy;



    vec2 rUV =
        fractureMap(
            uv,
            1.13
        );


    vec2 gUV =
        fractureMap(
            uv,
            5.71
        );


    vec2 bUV =
        fractureMap(
            uv,
            9.37
        );


    vec3 col =
        vec3(
            texture2D(
                bfr,
                rUV
            ).r,

            texture2D(
                bfr,
                gUV
            ).g,

            texture2D(
                bfr,
                bUV
            ).b
        );



    float t =
        time *
        mix(
            0.25,
            12.0,
            par_a.x
        );


    float levels =
        2.0 +
        floor(
            (
                0.5 +
                0.5 *
                sin(t * 1.73)
            )
            *
            14.0
        );


    col =
        floor(
            col * levels +
            0.5
        )
        /
        levels;



    vec2 zone =
        floor(
            uv *
            vec2(
                31.0,
                19.0
            )
        );


    float damage =
        hash21(
            zone +
            floor(t * 13.0)
        );


    if (damage > 0.82)
    {
        float threshold =
            0.15 +
            0.70 *
            hash21(
                zone.yx +
                floor(t * 7.0)
            );

        col =
            step(
                vec3(threshold),
                col
            );
    }



    float kill =
        hash21(
            zone * 3.17 +
            floor(t * 23.0)
        );


    if (kill > 0.93)
    {
        col.r = 0.0;
    }
    else if (kill > 0.86)
    {
        col.g = 0.0;
    }
    else if (kill > 0.79)
    {
        col.b = 0.0;
    }


    gl_FragColor =
        vec4(
            col,
            color.a
        );
}