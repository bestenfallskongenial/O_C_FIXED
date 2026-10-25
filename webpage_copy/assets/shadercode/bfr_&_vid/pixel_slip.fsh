precision mediump float;

// PIXEL SLIP
//
// Selected horizontal strips of vid slip sideways and wrap around the frame. bfr can remain inside the displaced strips, leaving fragments of earlier pictures caught between otherwise fresh rows.
//
// par_a.x - horizontal slip distance.
// par_a.y - number of horizontal strips.
// par_a.z - backbuffer retention within active strips.
// par_a.w - frequency of changes to the strip pattern.

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

    float amount   = par_a.x;
    float density  = par_a.y;
    float feedback = par_a.z;
    float speed    = par_a.w;


    float bands = floor(
        mix(6.0, 80.0, density)
    );

    float strip = floor(uv.y * bands);


    float tick = floor(
        time * mix(1.0, 18.0, speed)
    );

    float rnd = hash(
        vec2(strip + seed.x, tick)
    );

    float active = step(0.65, rnd);

    float offset =
        (rnd - 0.5)
        * amount
        * 0.4
        * active;


    vec2 sourceUV = uv;

    sourceUV.x = fract(
        sourceUV.x + offset
    );

    vec3 video = texture2D(
        vid,
        sourceUV
    ).rgb;


    vec2 historyUV = uv;

    historyUV.x = fract(
        historyUV.x + offset * 0.85
    );

    vec3 history = texture2D(
        bfr,
        historyUV
    ).rgb;


    float retention =
        feedback * 0.85 * active;

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
