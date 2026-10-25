precision mediump float;

// INK ATTRACTOR
//
// Edges from vid seed dark marks onto a pale surface. A curling flow transports the accumulated ink through bfr, stretching it into wandering threads and tangled deposits that slowly evaporate.
//
// par_a.x - twisting of the ink transport field.
// par_a.y - distance the ink travels each frame.
// par_a.z - ink persistence.
// par_a.w - amount of new ink introduced from video edges.

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


void main(void)
{
    vec2 uv = tcoord;

    float aspect = tres.x / max(tres.y, 1.0);

    vec2 p = uv - 0.5;
    p.x *= aspect;

    float t = time * 0.23;


    float a = sin(
        p.y * 9.0 + t
        + sin(p.x * 7.0 - t * 0.7)
    );

    float b = cos(
        p.x * 8.0 - t * 0.8
        + sin(p.y * 11.0 + t)
    );

    vec2 velocity = vec2(a, b);


    float angle =
        (a * b) * par_a.x * 1.8;

    float cs = cos(angle);
    float sn = sin(angle);

    velocity = vec2(
        velocity.x * cs - velocity.y * sn,
        velocity.x * sn + velocity.y * cs
    );


    vec2 displacement =
        velocity *
        par_a.y * 0.018;

    displacement.x /= aspect;

    vec2 historyUV = clamp(
        uv - displacement,
        0.0,
        1.0
    );


    vec3 history = texture2D(
        bfr,
        historyUV
    ).rgb;

    float ink = 1.0 - dot(
        history,
        vec3(0.333333)
    );


    vec2 px = 1.0 / max(tres, vec2(1.0));

    vec3 video = texture2D(
        vid, uv
    ).rgb;

    vec3 vx = texture2D(
        vid,
        clamp(uv + vec2(px.x * 2.0, 0.0),
              0.0, 1.0)
    ).rgb;

    vec3 vy = texture2D(
        vid,
        clamp(uv + vec2(0.0, px.y * 2.0),
              0.0, 1.0)
    ).rgb;

    float edge =
        length(video - vx)
        + length(video - vy);


    float injection = smoothstep(
        0.08,
        0.45,
        edge
    );

    injection *= par_a.w * 0.35;


    float concentration = 0.5 + 0.5 *
        sin(
            p.x * 17.0
            + p.y * 13.0
            + a * 3.0
            - b * 2.0
        );

    injection *= concentration;


    ink *= mix(
        0.90,
        0.999,
        par_a.z
    );

    ink = clamp(
        ink + injection,
        0.0,
        1.0
    );


    vec3 result = vec3(1.0 - ink);

    gl_FragColor = vec4(
        result,
        color.a
    );
}
