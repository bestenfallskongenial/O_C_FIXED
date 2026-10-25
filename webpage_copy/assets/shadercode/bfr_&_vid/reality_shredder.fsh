precision mediump float;

// REALITY SHREDDER
//
// Diagonal ribbons cut through vid, sliding sideways, folding within themselves, and selectively reversing the picture. bfr appears along their folded regions, making the strips seem to expose remembered imagery beneath their surfaces.
//
// par_a.x - ribbon displacement, folding, mirroring, and edge shading.
// par_a.y - ribbon density.
// par_a.z - backbuffer contribution within the folds.
// par_a.w - ribbon movement and channel-swap update speed.

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
    return fract(sin(n * 127.13) * 43758.5453);
}

void main(void)
{
    vec2 uv = tcoord;

    float power = par_a.x;
    float density = par_a.y;
    float memory = par_a.z;
    float speed = par_a.w;

    float t = time * (0.2 + speed * 4.0);


    float count = mix(8.0, 65.0, density);

    float axis = uv.y + uv.x * 0.38;

    float ribbon = floor(axis * count);
    float local = fract(axis * count);

    float rnd = hash(ribbon + seed.x);


    float phase = t + rnd * 6.2831853;

    float fold = sin(phase);

    float depth = power *
        (0.3 + rnd * 0.7);


    float folded = abs(local * 2.0 - 1.0);

    float compression = mix(
        local,
        folded,
        depth
    );

    float delta = (compression - local) / count;


    vec2 q = uv;

    q.y += delta;

    q.x += fold * depth * 0.35;


    float reverse = step(0.5, rnd);

    vec2 mirrored = q;

    mirrored.x = 1.0 - mirrored.x;

    q = mix(
        q,
        mirrored,
        reverse * power
    );


    vec3 video = texture2D(
        vid,
        fract(q)
    ).rgb;


    vec2 oldUV = uv;

    oldUV.x +=
        fold * depth * 0.025;

    oldUV.y +=
        (local - 0.5) * depth * 0.025;

    vec3 old = texture2D(
        bfr,
        clamp(oldUV, 0.0, 1.0)
    ).rgb;


    float underside = smoothstep(
        0.1,
        0.9,
        folded
    );

    underside *= power * memory;

    vec3 result = mix(
        video,
        old,
        underside
    );


    float edge = smoothstep(
        0.0, 0.08,
        min(local, 1.0 - local)
    );

    float lighting =
        0.7 + 0.3 * fold;

    result *= mix(
        1.0,
        edge * lighting,
        power * 0.65
    );


    float flip = step(
        0.85,
        hash(ribbon + floor(t * 2.0))
    );

    result = mix(
        result,
        result.bgr,
        flip * power
    );


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
