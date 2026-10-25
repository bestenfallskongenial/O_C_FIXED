precision mediump float;

// MANDELBULB AUTOPSY
//
// A moving cross-section passes through a Mandelbulb field, revealing rounded islands, branching boundaries, and densely nested escape bands. Slow rotation changes the exposed structure, letting intricate edges emerge and disappear around quieter interior regions.
//
// par_a.x - cross-section position.
// par_a.y - cross-section tilt.
// par_a.z - Mandelbulb power.
// par_a.w - magnification; higher values move closer into the structure.

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

#define PI 3.14159265359


void main(void)
{
    vec2 uv = tcoord;

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / max(tres.y, 1.0);


    float zoom = mix(
        1.6,
        0.35,
        par_a.w
    );

    vec3 point = vec3(
        p * zoom,
        mix(-1.2, 1.2, par_a.x)
    );


    float tilt =
        (par_a.y - 0.5) * PI;

    float ct = cos(tilt);
    float st = sin(tilt);

    point = vec3(
        point.x,
        point.y * ct - point.z * st,
        point.y * st + point.z * ct
    );


    float rotation = time * 0.08;

    float cr = cos(rotation);
    float sr = sin(rotation);

    point = vec3(
        point.x * cr - point.z * sr,
        point.y,
        point.x * sr + point.z * cr
    );


    vec3 z = vec3(0.0);

    float power = mix(
        3.0,
        9.0,
        par_a.z
    );

    float escape = 0.0;
    float orbit = 100.0;
    float iterations = 0.0;

    // Fixed upper bound for GLSL ES 2.0.
    // No nested loops or raymarching.

    for (int i = 0; i < 8; i++)
    {
        float r = length(z);

        orbit = min(orbit, r);

        if (r > 2.0)
        {
            escape = 1.0;
            break;
        }

        float theta = acos(
            clamp(
                z.z / max(r, 0.00001),
                -1.0,
                1.0
            )
        );

        float phi = atan(z.y, z.x);

        float zr = pow(r, power);

        theta *= power;
        phi *= power;

        z = zr * vec3(
            sin(theta) * cos(phi),
            sin(theta) * sin(phi),
            cos(theta)
        );

        z += point;

        iterations += 1.0;
    }


    float depth = iterations / 8.0;

    float interior = 1.0 - escape;


    float radius = max(length(z), 1.0001);

    float smoothIteration =
        iterations + 1.0 -
        log2(max(log2(radius), 0.0001));

    float phase =
        smoothIteration * 0.11 +
        orbit * 0.35;


    vec3 exterior = 0.5 + 0.5 * cos(
        6.2831853 *
        (phase + vec3(
            0.02,
            0.28,
            0.58
        ))
    );

    exterior *=
        0.35 + depth * 0.65;


    float veins = 0.5 + 0.5 * sin(
        orbit * 28.0 +
        point.z * 9.0
    );

    vec3 interiorColor = mix(
        vec3(0.015, 0.025, 0.045),
        vec3(0.08, 0.25, 0.38),
        veins * 0.65
    );


    float boundary = smoothstep(
        0.65,
        1.0,
        depth
    ) * escape;

    exterior += boundary *
        vec3(0.35, 0.55, 0.7);


    vec3 result = mix(
        exterior,
        interiorColor,
        interior
    );


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
