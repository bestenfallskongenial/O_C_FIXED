precision mediump float;

// Raster Memory Scramble
//
// Rows and columns of bfr copy fragments from neighboring groups, breaking the picture into repeated raster sections. Coarse sampling roughens their interiors, while changing patches reduce to single-channel monochrome or hard thresholded shapes.
//
// par_a.x - overall scrambling speed.
// par_a.y - row density.
// par_a.z - column density.
// par_a.w - size of row groups used for copying.
// par_b.x - rate of source-row and source-column changes.
// par_b.y - size of column groups used for copying.
// par_b.z - horizontal sampling subdivisions inside cells.
// par_b.w - vertical sampling subdivisions inside cells.

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


vec2 syncMap(vec2 uv)
{
    float t =
        time *
        mix(0.25, 10.0, par_a.x);



    float rows =
        48.0 +
        floor(
            (
                0.5 +
                0.5 *
                sin(t * 0.43)
            )
            *
            mix(32.0, 300.0, par_a.y)
        );


    float cols =
        8.0 +
        floor(
            (
                0.5 +
                0.5 *
                cos(t * 0.71)
            )
            *
            mix(8.0, 80.0, par_a.z)
        );



    vec2 grid =
        vec2(cols, rows);

    vec2 p =
        uv * grid;

    vec2 cell =
        floor(p);

    vec2 local =
        fract(p);



    float rowSpan =
        2.0 +
        floor(
            (
                0.5 +
                0.5 *
                sin(t * 1.31)
            )
            *
            mix(2.0, 24.0, par_a.w)
        );


    float rowGroup =
        floor(cell.y / rowSpan);

    float rowStart =
        rowGroup * rowSpan;

    float rowEnd =
        min(
            rowStart + rowSpan,
            rows
        );

    float availableRows =
        max(
            1.0,
            rowEnd - rowStart
        );


    float tick =
        floor(
            t *
            mix(2.0, 37.0, par_b.x)
        );


    float rowRnd =
        hash21(
            vec2(
                rowGroup,
                tick
            )
        );


    float sourceRow =
        rowStart +
        floor(
            rowRnd *
            availableRows
        );



    float colSpan =
        2.0 +
        floor(
            (
                0.5 +
                0.5 *
                cos(
                    t * 1.73 +
                    rowGroup * 0.37
                )
            )
            *
            mix(1.0, 18.0, par_b.y)
        );


    float colGroup =
        floor(cell.x / colSpan);

    float colStart =
        colGroup * colSpan;

    float colEnd =
        min(
            colStart + colSpan,
            cols
        );

    float availableCols =
        max(
            1.0,
            colEnd - colStart
        );


    float colRnd =
        hash21(
            vec2(
                colGroup +
                rowGroup * 17.0,

                tick * 1.317
            )
        );


    float sourceCol =
        colStart +
        floor(
            colRnd *
            availableCols
        );



    float horizontalHold =
        1.0 +
        floor(
            mix(1.0, 20.0, par_b.z)
            *
            (
                0.2 +
                0.8 *
                abs(
                    sin(
                        t * 2.71 +
                        rowGroup
                    )
                )
            )
        );


    local.x =
        (
            floor(
                local.x *
                horizontalHold
            )
            +
            0.5
        )
        /
        horizontalHold;



    float verticalHold =
        1.0 +
        floor(
            mix(1.0, 8.0, par_b.w)
            *
            (
                0.25 +
                0.75 *
                abs(
                    cos(
                        t * 3.13 +
                        colGroup
                    )
                )
            )
        );


    local.y =
        (
            floor(
                local.y *
                verticalHold
            )
            +
            0.5
        )
        /
        verticalHold;


    return
        (
            vec2(
                sourceCol,
                sourceRow
            )
            +
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


    vec2 srcUV =
        syncMap(uv);


    vec3 col =
        texture2D(
            bfr,
            srcUV
        ).rgb;



    float t =
        time *
        mix(0.25, 10.0, par_a.x);


    vec2 band =
        floor(
            uv *
            vec2(
                9.0,
                47.0
            )
        );


    float state =
        hash21(
            band +
            floor(t * 11.0)
        );


    float damage =
        step(
            0.58,
            state
        );


    vec3 damaged;

    if (state < 0.70)
    {
        damaged =
            vec3(col.r);
    }
    else if (state < 0.85)
    {
        damaged =
            vec3(col.g);
    }
    else
    {
        damaged =
            vec3(col.b);
    }


    col =
        mix(
            col,
            damaged,
            damage
        );



    float crush =
        step(
            0.88,

            hash21(
                band.yx +
                floor(t * 17.0)
            )
        );


    float threshold =
        0.2 +
        0.6 *
        hash21(
            band +
            floor(t * 5.0)
        );


    vec3 hard =
        step(
            vec3(threshold),
            col
        );


    col =
        mix(
            col,
            hard,
            crush
        );


    gl_FragColor =
        vec4(
            col,
            color.a
        );
}