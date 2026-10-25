precision mediump float;

// RGB Memory Blitter
//
// The channels of bfr follow separate block-copying maps, pulling the picture into overlapping grids of displaced fragments. Narrow slices subdivide the copied details, while intermittent patches collapse into monochrome versions of a single channel.
//
// par_a.x - speed of the changing grids and copying patterns.
// par_a.y - grid density.
// par_a.z - horizontal size of the groups within which blocks are copied.
// par_a.w - vertical size of those groups.
// par_b.x - rate of changes to the selected source blocks.
// par_b.y - horizontal slicing density inside copied cells.
// par_b.z - frequency of monochrome patches.
// par_b.w - strength of the monochrome replacement.

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


vec2 blitMap(vec2 uv, float seed)
{

    float t =
        time *
        mix(0.25, 8.0, par_a.x);



    vec2 grid;

    grid.x =
        8.0 +
        floor(
            (
                0.5 +
                0.5 *
                sin(t * 0.71 + seed * 2.37)
            )
            *
            mix(8.0, 56.0, par_a.y)
        );

    grid.y =
        6.0 +
        floor(
            (
                0.5 +
                0.5 *
                cos(t * 0.53 + seed * 3.11)
            )
            *
            mix(6.0, 40.0, par_a.y)
        );

    grid =
        max(grid, vec2(2.0));



    vec2 p =
        uv * grid;

    vec2 cell =
        floor(p);

    vec2 local =
        fract(p);



    vec2 groupSize;

    groupSize.x =
        2.0 +
        floor(
            (
                0.5 +
                0.5 *
                sin(t * 1.17 + seed * 5.31)
            )
            *
            mix(1.0, 10.0, par_a.z)
        );

    groupSize.y =
        2.0 +
        floor(
            (
                0.5 +
                0.5 *
                cos(t * 0.91 + seed * 7.17)
            )
            *
            mix(1.0, 8.0, par_a.w)
        );


    vec2 group =
        floor(cell / groupSize);

    vec2 groupStart =
        group * groupSize;

    vec2 groupEnd =
        min(
            groupStart + groupSize,
            grid
        );

    vec2 span =
        max(
            groupEnd - groupStart,
            vec2(1.0)
        );



    float tick =
        floor(
            t *
            mix(2.0, 29.0, par_b.x)
        );


    vec2 rnd =
        vec2(
            hash21(
                group +
                vec2(
                    tick + seed * 17.0,
                    seed * 11.0
                )
            ),

            hash21(
                group.yx +
                vec2(
                    seed * 29.0,
                    tick * 1.731
                )
            )
        );


    vec2 sourceCell =
        groupStart +
        floor(rnd * span);



    float slices =
        1.0 +
        floor(
            mix(1.0, 16.0, par_b.y)
            *
            (
                0.3 +
                0.7 *
                abs(
                    sin(
                        t * 2.13 +
                        seed
                    )
                )
            )
        );

    local.x =
        (
            floor(local.x * slices)
            + 0.5
        )
        / slices;



    return
        (
            sourceCell +
            local
        )
        /
        grid;
}


void main()
{
    vec2 uv =
        gl_FragCoord.xy / tres.xy;



    vec2 uvR =
        blitMap(uv, 1.17);

    vec2 uvG =
        blitMap(uv, 4.31);

    vec2 uvB =
        blitMap(uv, 8.73);


    float r =
        texture2D(
            bfr,
            uvR
        ).r;

    float g =
        texture2D(
            bfr,
            uvG
        ).g;

    float b =
        texture2D(
            bfr,
            uvB
        ).b;


    vec3 col =
        vec3(r, g, b);



    float gate =
        step(
            mix(0.95, 0.45, par_b.z),

            hash21(
                floor(uv * 23.0) +
                floor(time * 13.0)
            )
        );


    float mode =
        hash21(
            floor(uv * 11.0) +
            floor(time * 7.0)
        );


    vec3 broken = col;

    if (mode < 0.333)
    {
        broken =
            vec3(col.r);
    }
    else if (mode < 0.666)
    {
        broken =
            vec3(col.g);
    }
    else
    {
        broken =
            vec3(col.b);
    }


    col =
        mix(
            col,
            broken,
            gate * par_b.w
        );


    gl_FragColor =
        vec4(
            col,
            color.a
        );
}