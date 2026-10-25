precision mediump float;

// Singularity Transit
//
// vid wraps around a radial tunnel, streaming toward or away from a wandering center. Twisting streaks and periodic reversals disturb the journey, while bfr stretches the motion into outward-drifting trails.
//
// par_a.x - center movement, twisting, travel reversal, streaks, and core glow.
// par_a.y - radial tunnel sampling depth.
// par_a.z - backbuffer blend and radial trail displacement.
// par_a.w - tunnel travel and animation speed.

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

#define PI 3.14159265

void main(void)
{
    vec2 uv = tcoord;

    float aspect = tres.x / max(tres.y, 1.0);

    vec2 p = uv * 2.0 - 1.0;
    p.x *= aspect;

    float power = par_a.x;
    float depth = par_a.y;
    float trails = par_a.z;
    float speed = par_a.w;

    float t = time * (0.25 + speed * 3.0);


    vec2 center = vec2(
        sin(t * 0.37),
        cos(t * 0.29)
    ) * power * 0.35;

    p -= center;

    float r = length(p);
    float a = atan(p.y, p.x);


    float z = 1.0 / (r + 0.08);

    float tunnel = z *
        mix(0.15, 2.5, depth);


    vec2 q;

    q.x = a / (2.0 * PI);

    q.y = tunnel - t;


    q.x += sin(
        tunnel * 0.8 - t
    ) * power * 0.15;

    q.x += tunnel * power * 0.08;


    float pulse = sin(
        t * 1.7
    ) * 0.5 + 0.5;

    q.y *= mix(
        1.0,
        -1.0,
        pulse * power
    );


    vec2 videoUV = fract(q);

    vec3 video = texture2D(
        vid,
        videoUV
    ).rgb;


    float illumination =
        0.4 + 0.6 *
        smoothstep(0.0, 2.0, tunnel);

    video *= illumination;


    vec2 direction =
        p / max(r, 0.001);

    vec2 historyUV =
        uv - direction *
        trails * 0.009;

    historyUV = clamp(
        historyUV, 0.0, 1.0
    );

    vec3 history = texture2D(
        bfr,
        historyUV
    ).rgb;


    vec3 result = mix(
        video,
        history,
        trails * 0.94
    );


    float streak = pow(
        abs(sin(a * 24.0 + tunnel * 3.0)),
        12.0
    );

    result +=
        video *
        streak *
        power *
        0.7;


    float core = exp(
        -r * 12.0
    );

    result += vec3(
        0.4, 0.65, 1.0
    ) * core * power;

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
