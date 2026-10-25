precision mediump float;

// GRAVITY WELL
//
// A wandering center twists and lenses vid around a dark core. Rippled distortions surround the center, while a narrow luminous ring catches image details. bfr adds rotating trails that gather around the moving well.
//
// par_a.x - strength of the lensing, ripples, and core treatment.
// par_a.y - twisting strength around the center.
// par_a.z - backbuffer retention and trail rotation.
// par_a.w - core size; larger values soften the central pull.

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

    float gravity  = par_a.x;
    float rotation = par_a.y;
    float memory   = par_a.z;
    float size     = par_a.w;


    float aspect = tres.x / max(tres.y, 1.0);


    vec2 center = vec2(
        0.5 + sin(time * 0.37) * 0.26,
        0.5 + cos(time * 0.53) * 0.24
    );

    vec2 p = uv - center;
    p.x *= aspect;

    float radius = length(p);


    float core = mix(0.015, 0.25, size);

    float influence =
        gravity / (radius * radius + core);

    influence = min(influence, 25.0);


    float pull =
        1.0 - exp(-influence * 0.065);

    float collapsedRadius =
        radius * (1.0 - pull * 0.92);


    float angle = atan(p.y, p.x);

    angle +=
        rotation
        * influence
        * 0.85;


    float tidal =
        sin(
            angle * 7.0
            - time * 2.0
            + radius * 34.0
        );

    collapsedRadius +=
        tidal
        * pull
        * gravity
        * 0.035;


    vec2 q = vec2(
        cos(angle),
        sin(angle)
    ) * collapsedRadius;

    q.x /= aspect;

    vec2 sourceUV = clamp(
        center + q,
        0.0,
        1.0
    );


    vec3 video =
        texture2D(vid, sourceUV).rgb;


    vec2 trail = uv - center;

    trail.x *= aspect;

    float trailAngle =
        memory
        * rotation
        * 0.035
        / (radius + 0.15);

    float cs = cos(trailAngle);
    float sn = sin(trailAngle);

    trail = vec2(
        cs * trail.x - sn * trail.y,
        sn * trail.x + cs * trail.y
    );

    trail *= 1.0 + gravity * 0.018;

    trail.x /= aspect;

    vec2 historyUV = clamp(
        center + trail,
        0.0,
        1.0
    );

    vec3 history =
        texture2D(bfr, historyUV).rgb;


    float retention =
        memory
        * (0.35 + pull * 0.64);

    vec3 result = mix(
        video,
        history,
        retention
    );


    float horizon =
        smoothstep(
            core * 0.08,
            core * 0.42,
            radius
        );

    result *= mix(
        1.0,
        horizon,
        gravity
    );


    float ringRadius =
        sqrt(core) * 0.24;

    float ring = exp(
        -abs(radius - ringRadius)
        * mix(80.0, 220.0, gravity)
    );

    float brightness =
        dot(video, vec3(0.299, 0.587, 0.114));

    result +=
        video
        * ring
        * gravity
        * (0.3 + brightness * 1.2);


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
