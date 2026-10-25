precision mediump float;

// Digital Fault Matrix
//
// Checker fragments and narrow raster bars accumulate through bfr. Selected rows slip sideways, occasional jumps disturb the whole picture vertically, and channel exchanges interrupt the growing matrix like faults spreading through a digital display.
//
// par_a.x - horizontal density of the generated cells.
// par_a.y - vertical density of the generated cells.
// par_a.z - update rate of the changing cells and tear events.
// par_b.x - backbuffer persistence.
// par_b.y - strength of newly generated patterns.
// par_b.z - strength of channel exchanges in selected rows.

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

float hash2(vec2 p)
{
    return fract(
        sin(dot(p, vec2(12.9898, 78.233)))
        * 43758.5453
    );
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    // coarse digital cells

    float gridX = mix(12.0, 64.0, par_a.x);
    float gridY = mix(8.0,  32.0, par_a.y);

    vec2 cell =
        floor(uv * vec2(gridX, gridY));

    float frame =
        floor(time * mix(4.0, 20.0, par_a.z));

    float rnd =
        hash2(cell + vec2(frame, -frame));

    // feedback tearing

    vec2 fuv = uv;

    float row =
        floor(uv.y * 32.0);

    float rowRnd =
        hash2(vec2(row, frame));

    fuv.x +=
        step(0.80, rowRnd) *
        (rowRnd - 0.5) *
        0.14;

    // vertical sync-style jumping

    float jump =
        step(0.96,
             hash2(vec2(frame, frame * 0.17)));

    fuv.y +=
        jump *
        sin(time * 31.0) *
        0.08;

    fuv = fract(fuv);

    vec3 old =
        texture2D(bfr, fuv).rgb;

    // procedural digital source

    float checker =
        mod(cell.x + cell.y + frame, 2.0);

    float bar =
        step(
            0.87,
            sin(
                uv.y * 220.0 +
                time * 17.0
            )
        );

    float pulse =
        step(0.73, rnd);

    vec3 proc;

    proc.r = checker * pulse;
    proc.g = bar * (1.0 - checker);
    proc.b = pulse * (1.0 - bar);

    // temporal corruption

    float persistence =
        mix(0.86, 0.998, par_b.x);

    vec3 col =
        old * persistence +
        proc * mix(0.02, 0.28, par_b.y);

    // random channel permutation

    float channelGlitch =
        step(0.92, rowRnd);

    col = mix(
        col,
        col.gbr,
        channelGlitch * par_b.z
    );

    gl_FragColor =
        vec4(clamp(col, 0.0, 1.0), color.a);
}