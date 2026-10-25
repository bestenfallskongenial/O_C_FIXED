precision mediump float;

// ACID EROSION
//
// Bright remnants spread through neighboring areas, eating into the picture like a restless chemical stain. vid supplies fresh imagery while bfr carries the corroding residue forward. Changes in the picture open passages through the accumulated surface.
//
// par_a.x - erosion strength and sensitivity of the spreading reaction.
// par_a.y - spread distance and grain density of the reaction.
// par_a.z - persistence of the residue; higher values admit less fresh video.

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

float luminance(vec3 c)
{
    return dot(c, vec3(0.299, 0.587, 0.114));
}

void main(void)
{
    vec2 uv = tcoord;

    float acid   = par_a.x;
    float scale  = par_a.y;
    float memory = par_a.z;


    vec3 video = texture2D(vid, uv).rgb;


    vec3 old = texture2D(bfr, uv).rgb;


    vec2 px = vec2(1.0) / max(tres, vec2(1.0));

    vec2 spread = px * mix(1.0, 28.0, scale);

    vec3 a = texture2D(
        bfr, clamp(uv + vec2(spread.x, 0.0), 0.0, 1.0)
    ).rgb;

    vec3 b = texture2D(
        bfr, clamp(uv - vec2(spread.x, 0.0), 0.0, 1.0)
    ).rgb;

    vec3 c = texture2D(
        bfr, clamp(uv + vec2(0.0, spread.y), 0.0, 1.0)
    ).rgb;

    vec3 d = texture2D(
        bfr, clamp(uv - vec2(0.0, spread.y), 0.0, 1.0)
    ).rgb;


    float e = max(
        max(luminance(a), luminance(b)),
        max(luminance(c), luminance(d))
    );

    float current = luminance(video);
    float previous = luminance(old);


    float noise = hash(
        floor(uv * mix(24.0, 240.0, scale))
        + floor(time * 7.0) * 0.13
        + seed.xy
    );

    float threshold =
        mix(0.85, 0.18, acid)
        + (noise - 0.5) * 0.22;

    float reaction = smoothstep(
        threshold - 0.12,
        threshold + 0.12,
        e * 0.75 + current * 0.25
    );


    vec3 dissolved = (a + b + c + d) * 0.25;

    dissolved = mix(
        dissolved,
        vec3(
            dissolved.g * 1.5,
            dissolved.b * 0.65,
            dissolved.r * 0.25
        ),
        acid * 0.8
    );


    float decay =
        mix(0.75, 0.995, memory);

    vec3 residue = old * decay;

    residue = mix(
        residue,
        dissolved,
        reaction * acid * 0.45
    );


    float holes = smoothstep(
        0.15,
        0.85,
        abs(current - previous) * (1.0 + acid * 4.0)
    );

    float exposure =
        mix(0.8, 0.015, memory);

    exposure += holes * (1.0 - memory) * 0.35;

    vec3 result = mix(
        residue,
        video,
        clamp(exposure, 0.0, 1.0)
    );


    float edge = abs(
        reaction - smoothstep(
            threshold,
            threshold + 0.2,
            previous
        )
    );

    result += vec3(
        0.15,
        0.8,
        0.35
    ) * edge * acid * 0.65;


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
