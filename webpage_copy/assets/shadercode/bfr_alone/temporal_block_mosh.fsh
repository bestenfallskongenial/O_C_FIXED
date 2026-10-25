precision mediump float;

// Temporal Block Mosh
//
// Blocks of bfr follow independent oscillating paths, reversing direction and occasionally bursting into larger jumps. Their changing sizes and intermittent channel exchanges continually dismantle and reassemble the retained picture.
//
// par_a.x - update rate of block sizes and displacement bursts.
// par_a.y - upper range of block sizes.
// par_a.z - displacement strength.

uniform float time;
uniform vec2 tres;
uniform vec4 par_a;
uniform vec4 par_b;
uniform vec4 color;

uniform sampler2D bfr;

float hash(float x)
{
    return fract(sin(x * 91.3458) * 47453.5453);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / tres.xy;


    float t =
        time * (2.0 + par_a.x * 20.0);

    float tick =
        floor(t);


    float bs =
        4.0 +
        floor(
            hash(
                tick * 0.731 +
                sin(time * 0.37) * 11.0
            )
            *
            mix(8.0, 80.0, par_a.y)
        );

    vec2 cell =
        floor(gl_FragCoord.xy / bs);

    vec2 local =
        mod(gl_FragCoord.xy, bs);


    float blockID =
        cell.x * 13.7 +
        cell.y * 31.1;

    float phase =
        time * 2.31 +
        blockID;


    vec2 motion =
        vec2(
            sin(phase * 0.73),
            cos(phase * 0.91)
        );

    motion *=
        mix(2.0, 70.0, par_a.z);


    float reverse =
        step(
            0.0,
            sin(
                time * 3.17 +
                blockID * 0.27
            )
        );

    motion *=
        mix(
            -1.0,
             1.0,
             reverse
        );


    float burst =
        step(
            0.82,
            hash(
                blockID +
                tick * 7.13
            )
        );

    motion *=
        mix(
            1.0,
            5.0,
            burst
        );


    vec2 srcPixel =
        cell * bs +
        local -
        motion;

    vec2 srcUV =
        fract(
            srcPixel / tres.xy
        );

    vec3 mosh =
        texture2D(
            bfr,
            srcUV
        ).rgb;


    float keep =
        fract(
            sin(
                time * 4.71 +
                blockID * 0.113
            )
            * 43758.5453
        );

    vec3 mutate =
        mix(
            mosh,
            mosh.gbr,
            step(0.68, keep)
        );

    gl_FragColor =
        vec4(
            mutate,
            color.a
        );
}