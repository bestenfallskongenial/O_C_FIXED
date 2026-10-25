precision mediump float;

// FRACTURED MEMORY
//
// The picture tears into displaced rows and restless blocks. bfr both steers the deformation and supplies lingering fragments, while vid breaks through in shifting patches. Channels split, blocks collapse, and intermittent recoveries briefly restore the moving image.
//
// par_a.x - tearing, displacement, block collapse, and channel disruption strength.
// par_a.y - density of the blocks and horizontal tear bands.
// par_a.z - persistence of backbuffer fragments.
// par_a.w - mutation rate, block activity, and additional distortion.

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


float hash(vec2 p)
{
    return fract(
        sin(dot(p, vec2(127.1, 311.7)))
        * 43758.5453
    );
}

void main(void)
{
    vec2 uv = tcoord;

    float distortion = par_a.x;
    float fragments  = par_a.y;
    float persistence = par_a.z;
    float chaos = par_a.w;


    float tick = floor(
        time * mix(2.0, 32.0, chaos)
    );


    vec2 grid = vec2(
        floor(mix(6.0, 72.0, fragments)),
        floor(mix(4.0, 48.0, fragments))
    );

    vec2 cell = floor(uv * grid);

    float rnd = hash(
        cell + vec2(tick, -tick) + seed.xy
    );

    float rnd2 = hash(
        cell.yx + vec2(-tick, tick * 0.37)
    );


    float row = floor(
        uv.y * mix(9.0, 110.0, fragments)
    );

    float rowNoise = hash(
        vec2(row, tick + seed.z)
    );

    float tear = step(
        1.0 - distortion * 0.8,
        rowNoise
    );

    float horizontalShift =
        (rowNoise - 0.5)
        * distortion
        * 0.75
        * tear;


    vec2 probeUV =
        (cell + 0.5) / grid;

    vec3 control =
        texture2D(bfr, probeUV).rgb;

    vec2 motion =
        (control.rg * 2.0 - 1.0)
        * distortion
        * 0.20;

    float blockActive = step(
        0.75 - chaos * 0.55,
        rnd
    );

    motion *= blockActive;


    vec2 warped = uv;

    warped.x += horizontalShift;
    warped += motion;

    warped.x +=
        sin(
            warped.y * 23.0
            + time * 1.7
            + control.b * 12.0
        )
        * distortion
        * 0.065;

    warped.y +=
        sin(
            warped.x * 34.0
            - time * 2.1
        )
        * distortion
        * chaos
        * 0.045;


    float collapse = step(
        1.0 - distortion * 0.6,
        rnd2
    );

    vec2 snapped =
        (floor(warped * grid) + 0.5)
        / grid;

    warped = mix(
        warped,
        snapped,
        collapse
    );

    warped = fract(warped);


    vec3 history =
        texture2D(bfr, warped).rgb;


    vec2 videoUV = fract(
        warped + vec2(
            horizontalShift * 0.3,
            motion.y * 0.45
        )
    );

    vec3 live =
        texture2D(vid, videoUV).rgb;


    float contamination = step(
        0.48,
        hash(
            vec2(
                floor(time * 3.0),
                floor(row * 0.25)
            )
        )
    );

    float retention =
        persistence
        * mix(0.72, 0.995, contamination);

    vec3 result = mix(
        live,
        history,
        retention
    );


    vec2 chromaOffset = vec2(
        distortion * chaos * 0.045,
        distortion * 0.018
    );

    vec3 broken;

    broken.r = texture2D(
        bfr,
        fract(warped + chromaOffset)
    ).r;

    broken.g = result.g;

    broken.b = texture2D(
        vid,
        fract(videoUV - chromaOffset)
    ).b;

    float chromaMask = step(
        1.0 - distortion * 0.8,
        rnd
    );

    result = mix(
        result,
        broken,
        chromaMask
    );


    float mutation = step(
        1.0 - chaos * 0.65,
        hash(cell + vec2(tick * 0.71))
    );

    vec3 mutant = vec3(
        result.g,
        1.0 - result.b,
        result.r
    );

    result = mix(
        result,
        mutant,
        mutation * distortion
    );


    float recovery = step(
        0.93,
        hash(
            vec2(
                floor(time * 8.0),
                row
            )
        )
    );

    result = mix(
        result,
        live,
        recovery * (1.0 - persistence * 0.85)
    );


    result +=
        (history - 0.5)
        * distortion
        * persistence
        * 0.14;

    result = clamp(result, 0.0, 1.0);


    gl_FragColor = vec4(result, color.a);
}
