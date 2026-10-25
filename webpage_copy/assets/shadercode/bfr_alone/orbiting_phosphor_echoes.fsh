precision mediump float;

// Orbiting Phosphor Echoes
//
// Two wandering rings draw luminous paths across the frame. bfr retains and gently transforms their earlier positions, building overlapping loops and drifting trails like phosphor slowly releasing the memory of a moving beam.
//
// par_a.x - trail persistence.
// par_a.y - feedback coordinate scale; higher values contract the retained image.
// par_a.z - feedback rotation bias, with a small automatic oscillation.
// par_a.w - size of the generated rings.
// par_b.x - movement speed of the rings.

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

#define PI 3.141592654

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    // feedback coordinate

    vec2 fp = uv - 0.5;

    float angle =
        (par_a.z - 0.5) * 0.03 +
        sin(time * 0.13) * 0.004;

    float s = sin(angle);
    float c = cos(angle);

    fp = mat2(c, -s,
              s,  c) * fp;

    float zoom = mix(0.990, 1.010, par_a.y);

    fp *= zoom;

    vec3 old =
        texture2D(bfr, fp + 0.5).rgb;

    // procedural coordinate

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / tres.y;

    // autonomous moving generators

    float speed = mix(0.2, 2.0, par_b.x);

    vec2 a = vec2(
        sin(time * 0.73 * speed),
        cos(time * 0.91 * speed)
    ) * 0.55;

    vec2 b = vec2(
        cos(time * 0.51 * speed),
        sin(time * 1.13 * speed)
    ) * 0.45;

    float da = length(p - a);
    float db = length(p - b);

    // two moving rings

    float radius =
        mix(0.03, 0.20, par_a.w);

    float ringA =
        1.0 -
        smoothstep(
            0.01,
            0.035,
            abs(da - radius)
        );

    float ringB =
        1.0 -
        smoothstep(
            0.01,
            0.035,
            abs(db - radius * 0.65)
        );

    // time-dependent procedural colour

    vec3 ca = vec3(
        0.5 + 0.5 * sin(time * 0.31),
        0.5 + 0.5 * sin(time * 0.37 + 2.0),
        0.5 + 0.5 * sin(time * 0.43 + 4.0)
    );

    vec3 cb = ca.brg;

    vec3 generated =
        ca * ringA +
        cb * ringB;

    // temporal feedback

    float persistence =
        mix(0.80, 0.995, par_a.x);

    vec3 col =
        old * persistence +
        generated * 0.35;

    gl_FragColor =
        vec4(clamp(col, 0.0, 1.0), color.a);
}