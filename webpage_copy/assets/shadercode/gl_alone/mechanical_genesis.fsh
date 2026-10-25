precision mediump float;

// MECHANICAL GENESIS
//
// Polygonal frames, radial arms, and segmented rings turn around a pulsing core. The assembly repeatedly expands and gathers inward, like a delicate mechanism testing the reach of its moving parts.
//
// par_a.x - number of polygon sides, ring segments, and radial arms.
// par_a.y - extent of the rhythmic expansion.
// par_a.z - animation speed.
// par_a.w - brightness of illuminated outlines and core.

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


float polygon(vec2 p, float sides)
{
    float a = atan(p.y, p.x);
    float sector = 2.0 * PI / sides;

    return length(p) * cos(
        floor(0.5 + a / sector) * sector - a
    );
}

float ring(float d, float r, float w)
{
    return 1.0 - smoothstep(
        w, w + 0.012, abs(d - r)
    );
}

void main(void)
{
    vec2 uv = tcoord;

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / max(tres.y, 1.0);

    float speed = 0.1 + par_a.z * 1.5;
    float t = time * speed;


    float assembly =
        0.5 + 0.5 * sin(t * 0.7);

    float expansion =
        par_a.y * assembly * 0.45;

    float complexity = par_a.x;
    float glow = par_a.w;

    float radius = length(p);
    float angle = atan(p.y, p.x);


    float a1 = t * 0.25;

    vec2 q1 = vec2(
        p.x * cos(a1) - p.y * sin(a1),
        p.x * sin(a1) + p.y * cos(a1)
    );

    float sides = floor(
        mix(5.0, 12.0, complexity)
    );

    float chassis = polygon(q1, sides);

    float outer = ring(
        chassis,
        0.72 + expansion,
        0.018
    );

    float inner = ring(
        chassis,
        0.60 + expansion * 0.7,
        0.012
    );


    float a2 = -t * 0.43;

    vec2 q2 = vec2(
        p.x * cos(a2) - p.y * sin(a2),
        p.x * sin(a2) + p.y * cos(a2)
    );

    float innerPoly = polygon(
        q2,
        sides + 3.0
    );

    float mechanism = ring(
        innerPoly,
        0.38 + expansion * 0.3,
        0.014
    );


    float segments = floor(
        mix(12.0, 48.0, complexity)
    );

    float segmentation = step(
        0.25,
        fract(angle / (2.0 * PI) *
              segments + t * 0.3)
    );

    float segmentedRing = ring(
        radius,
        0.83 + expansion * 0.5,
        0.009
    ) * segmentation;


    float arms = floor(
        mix(4.0, 16.0, complexity)
    );

    float armAngle =
        angle * arms + t * 0.4;

    float armPattern = pow(
        abs(cos(armAngle * 0.5)),
        24.0
    );

    float armRadius = smoothstep(
        0.20,
        0.28,
        radius
    ) * (1.0 - smoothstep(
        0.58 + expansion,
        0.62 + expansion,
        radius
    ));

    float mechanicalArms =
        armPattern * armRadius;


    float plate = 1.0 - smoothstep(
        0.0,
        0.025,
        abs(innerPoly - 0.43)
    );

    float depthShade =
        0.5 + 0.5 * sin(
            angle * sides - t
        );


    float core = exp(
        -radius * 14.0
    );

    float reactor = ring(
        radius,
        0.16 + sin(t * 1.7) * 0.025,
        0.012
    );


    vec3 result = vec3(
        0.005,
        0.012,
        0.025
    );


    vec3 metal = vec3(
        0.12,
        0.24,
        0.32
    );

    float structure =
        outer +
        inner +
        mechanism +
        mechanicalArms * 0.65;

    result += metal * structure;

    result += metal *
        plate *
        depthShade *
        0.5;


    vec3 cyan = vec3(
        0.02,
        0.75,
        1.0
    );

    vec3 amber = vec3(
        1.0,
        0.35,
        0.06
    );

    result += cyan *
        (outer + mechanism + segmentedRing)
        * glow;

    result += amber *
        (inner + mechanicalArms)
        * glow * 0.7;


    result += cyan *
        reactor *
        glow;

    result += vec3(
        0.3,
        0.7,
        1.0
    ) * core * glow;


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
