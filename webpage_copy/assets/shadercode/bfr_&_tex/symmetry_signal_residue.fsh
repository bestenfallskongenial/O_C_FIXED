precision mediump float;

// Symmetry Signal Residue
//
// Mirrored wave shapes overlap a gently warped source texture. bfr gathers their earlier positions into turning, expanding or contracting residues, letting crisp symmetric boundaries dissolve into a layered field of remembered forms.
//
// par_a.x - backbuffer persistence.
// par_a.y - feedback rotation bias, with a small automatic oscillation.
// par_a.z - feedback coordinate scale; higher values contract the retained image.
// par_a.w - strength of the source-texture contribution.
// par_b.x - strength of the generated symmetric pattern.
// par_b.y - frequency of the symmetric waves.
// par_b.z - distortion strength of the source texture.

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

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;


    vec2 fp = uv - 0.5;

    float angle =
        (par_a.y - 0.5) * 0.035;

    // slow autonomous wobble
    angle += sin(time * 0.21) * 0.005;

    float s = sin(angle);
    float c = cos(angle);

    fp = mat2(c, -s,
              s,  c) * fp;

    fp *= mix(0.985, 1.015, par_a.z);

    fp += 0.5;

    vec3 old =
        texture2D(bfr, fp).rgb;


    vec2 p =
        gl_FragCoord.xy / tres.xy * 2.0 - 1.0;

    p.x *= tres.x / tres.y;

    p = abs(p);

    float freq =
        mix(2.0, 12.0, par_b.y);

    vec3 procedural = vec3(0.0);

    procedural += smoothstep(
        0.20,
        0.24,
        sin(
            p.x * freq +
            time * vec3(1.0, 1.7, 2.3)
        ) +
        0.5 -
        p.y
    );

    procedural += smoothstep(
        0.20,
        0.24,
        sin(
            p.y * freq * 0.7 -
            time * vec3(1.3, 1.9, 2.7)
        ) *
        0.5 +
        1.0 -
        p.x
    );

    procedural =
        abs(1.0 - procedural);


    vec2 tuv = uv;

    float warp =
        par_b.z * 0.025;

    tuv.x +=
        sin(
            uv.y * 14.0 +
            time * 0.8
        ) * warp;

    tuv.y +=
        cos(
            uv.x * 11.0 -
            time * 0.6
        ) * warp;

    vec3 source =
        texture2D(tex[0], tuv).rgb;


    float persistence =
        mix(0.80, 0.995, par_a.x);

    float textureInjection =
        mix(0.0, 0.08, par_a.w);

    float proceduralInjection =
        mix(0.0, 0.25, par_b.x);

    vec3 col =
          old        * persistence
        + source     * textureInjection
        + procedural * proceduralInjection;


    gl_FragColor =
        vec4(clamp(col, 0.0, 1.0), color.a);
}