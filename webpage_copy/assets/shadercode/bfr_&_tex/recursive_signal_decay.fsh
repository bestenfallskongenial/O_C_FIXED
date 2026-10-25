precision mediump float;

// Recursive Signal Decay
//
// A torn source texture mixes with raster marks and blocklike signal fragments. bfr preserves their displaced echoes, allowing fresh interruptions to accumulate into a flickering surface of slipped rows, channel exchanges, and persistent debris.
//
// par_a.x - horizontal tearing of the retained image.
// par_a.y - feedback coordinate scale; higher values contract the retained image.
// par_a.z - blockwise horizontal tearing of the source texture.
// par_a.w - strength of the source texture’s horizontal wave distortion.
// par_b.x - backbuffer persistence.
// par_b.y - strength of the source-texture contribution.
// par_b.z - strength of generated raster and block patterns.

varying vec2 tcoord;

uniform float time;
uniform vec2  tres;

uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;

uniform sampler2D tex[8];
uniform int tex_l;

uniform sampler2D bfr;

float hash3(vec2 p)
{
    return fract(
        sin(dot(p, vec2(127.1, 311.7)))
        * 43758.5453
    );
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    float frame =
        floor(time * 10.0);

    // block coordinates

    vec2 blocks =
        floor(uv * vec2(24.0, 16.0));

    float rnd =
        hash3(blocks + frame);

    // feedback coordinate

    vec2 fuv = uv;

    float band =
        floor(uv.y * 30.0);

    float bandRnd =
        hash3(vec2(band, frame));

    // horizontal frame tearing
    fuv.x +=
        step(0.74, bandRnd) *
        (bandRnd - 0.5) *
        mix(0.01, 0.18, par_a.x);

    // slight recursive zoom
    vec2 fp = fuv - 0.5;

    fp *= mix(0.985, 1.015, par_a.y);

    fuv = fp + 0.5;

    fuv = clamp(fuv, 0.0, 1.0);

    vec3 old =
        texture2D(bfr, fuv).rgb;

    // source texture

    vec2 suv = uv;

    float texTear =
        step(0.86, rnd) *
        (rnd - 0.5) *
        par_a.z *
        0.35;

    suv.x += texTear;

    // time-controlled wave corruption
    suv.x +=
        sin(
            uv.y * 55.0 +
            time * 7.0
        ) *
        0.005 *
        par_a.w;

    suv = clamp(suv, 0.0, 1.0);

    vec3 src =
        texture2D(tex[0], suv).rgb;

    // procedural corruption source

    float scan =
        step(
            0.80,
            sin(
                gl_FragCoord.y * 0.31 +
                time * 13.0
            )
        );

    float digital =
        step(0.82, rnd);

    vec3 proc =
        vec3(
            digital,
            scan * digital,
            scan
        );

    // audio can punch the corruption harder

    float audioDrive =
        0.25 *
        (audio.x + audio.y + audio.z + audio.w);

    // combine

    vec3 col =
          old * mix(0.86, 0.997, par_b.x)
        + src * mix(0.005, 0.16, par_b.y)
        + proc * (
            mix(0.005, 0.20, par_b.z)
            + audioDrive * 0.08
          );

    // block-level channel scrambling

    float scramble =
        step(0.91, rnd);

    col = mix(
        col,
        col.brg,
        scramble
    );

    gl_FragColor =
        vec4(clamp(col, 0.0, 1.0), color.a);
}