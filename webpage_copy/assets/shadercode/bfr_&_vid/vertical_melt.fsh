precision mediump float;

// VERTICAL MELT
//
// Columns of vid stretch and slide vertically at different rates, breaking the picture into uneven streams. bfr leaves slower remnants behind the moving columns, thickening the impression of a surface beginning to melt.
//
// par_a.x - vertical stretching, drift strength, and backbuffer displacement.
// par_a.y - number of columns; higher values make them narrower.
// par_a.z - backbuffer retention.
// par_a.w - vertical drift speed.

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


float hash(float n)
{
    return fract(sin(n * 127.1) * 43758.5453);
}

void main(void)
{
    vec2 uv = tcoord;

    float amount = par_a.x;
    float width  = par_a.y;
    float memory = par_a.z;
    float speed  = par_a.w;


    float columns = floor(
        mix(8.0, 96.0, width)
    );

    float column = floor(uv.x * columns);

    float rnd = hash(column + seed.x);


    float velocity = mix(
        0.15, 1.0, rnd
    );

    float phase = time *
        mix(0.05, 1.5, speed) *
        velocity;

    float drift = fract(phase + rnd);


    float stretch =
        amount * (0.2 + rnd * 0.8);

    vec2 sourceUV = uv;

    sourceUV.y = fract(
        uv.y * (1.0 - stretch * 0.75)
        - drift * stretch
    );


    vec3 video = texture2D(
        vid,
        sourceUV
    ).rgb;


    vec2 historyUV = uv;

    historyUV.y = clamp(
        uv.y - amount * velocity * 0.008,
        0.0,
        1.0
    );

    vec3 history = texture2D(
        bfr,
        historyUV
    ).rgb;


    float retention =
        memory * (0.3 + rnd * 0.65);

    vec3 result = mix(
        video,
        history,
        retention
    );

    gl_FragColor = vec4(
        result,
        color.a
    );
}
