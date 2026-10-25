precision mediump float;

// FRACTAL ARTIFACT
//
// A rotating, faceted emblem gathers nested contours inside a circular enclosure. Segmented outer rings and a luminous center frame the folded body, making it resemble an unfamiliar instrument scanning its own internal structure.
//
// par_a.x - fractal folding power and number of facets.
// par_a.y - body extent and surface deformation.
// par_a.z - rotation and scanning speed.
// par_a.w - brightness of contours, surrounding markers, and core glow.

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


vec3 bulbFold(vec3 z, float power)
{
    float r = max(length(z), 0.0001);

    float theta = acos(
        clamp(z.z / r, -1.0, 1.0)
    );

    float phi = atan(z.y, z.x);

    float radius = pow(
        min(r, 2.0),
        power
    );

    theta *= power;
    phi *= power;

    return radius * vec3(
        sin(theta) * cos(phi),
        sin(theta) * sin(phi),
        cos(theta)
    );
}

void main(void)
{
    vec2 uv = tcoord;

    float aspect = tres.x / max(tres.y, 1.0);

    vec2 p = uv * 2.0 - 1.0;
    p.x *= aspect;

    float t = time * (0.1 + par_a.z);


    float cs = cos(t);
    float sn = sin(t);

    vec2 q = vec2(
        p.x * cs - p.y * sn,
        p.x * sn + p.y * cs
    );

    float radius = length(q);


    float sphere = sqrt(
        max(0.0, 1.0 - radius * radius)
    );

    vec3 z = vec3(
        q * 0.85,
        sphere * 0.7
    );

    vec3 c = z;


    float power = mix(
        2.0, 5.0, par_a.x
    );

    z = bulbFold(z, power) * 0.55 + c;
    z = bulbFold(z, power) * 0.45 + c;
    z = bulbFold(z, power) * 0.35 + c;


    float angle = atan(z.y, z.x);

    float sectors = floor(
        mix(5.0, 16.0, par_a.x)
    );

    float facet = cos(
        floor(angle / (2.0 * PI) *
              sectors + 0.5)
        * (2.0 * PI / sectors)
        - angle
    );

    float geometry =
        length(z.xy) * facet;

    geometry +=
        sin(z.z * 8.0 + t) *
        par_a.y * 0.12;


    float boundary = mix(
        0.45,
        0.85,
        par_a.y
    );

    float body = 1.0 - smoothstep(
        boundary - 0.025,
        boundary + 0.025,
        geometry
    );

    body *= 1.0 - smoothstep(
        0.85,
        1.0,
        radius
    );


    float contour = abs(
        fract(geometry * 9.0) - 0.5
    );

    float lines = 1.0 - smoothstep(
        0.02,
        0.08,
        contour
    );


    float ring = 1.0 - smoothstep(
        0.006,
        0.018,
        abs(radius - 0.92)
    );

    float ring2 = 1.0 - smoothstep(
        0.005,
        0.014,
        abs(radius - 1.08)
    );


    float ticks = step(
        0.85,
        cos(angle * 48.0)
    );

    float targeting =
        ring * ticks + ring2 * 0.35;


    float scan = 0.5 + 0.5 *
        sin(
            z.z * 12.0
            + geometry * 18.0
            - t * 3.0
        );


    vec3 dark = vec3(
        0.015, 0.025, 0.055
    );

    vec3 metal = vec3(
        0.07, 0.18, 0.28
    );

    vec3 cyan = vec3(
        0.05, 0.85, 1.0
    );

    vec3 result = mix(
        dark,
        metal * (0.5 + scan),
        body
    );


    result += cyan *
        lines *
        body *
        par_a.w;

    result += cyan *
        targeting *
        par_a.w;


    float core = exp(
        -radius * 18.0
    );

    result += vec3(
        0.25, 0.65, 1.0
    ) * core * par_a.w;


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
