precision mediump float;

// TOPOLOGICAL BLOOM
//
// A radial bloom unfolds into twisted petals, nested chambers, and fine branching veins. vid textures its interior, while bfr can preserve brighter remnants when the pattern or source changes.
//
// par_a.x - petal symmetry, from 4 to 17, and density of nested chambers.
// par_a.y - twisting and deformation of the bloom.
// par_a.z - influence of video on the interior texture.
// par_a.w - strength of retained backbuffer remnants.

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


vec3 spectrum(float x)
{
    return 0.5 + 0.5 * cos(
        6.2831853 *
        (x + vec3(0.0, 0.29, 0.63))
    );
}

void main(void)
{
    vec2 uv = tcoord;

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / max(tres.y, 1.0);

    float radius = length(p);
    float angle = atan(p.y, p.x);

    float symmetry = floor(
        mix(4.0, 17.0, par_a.x)
    );

    float deformation = par_a.y;


    float twist =
        angle +
        radius * (1.0 + deformation * 7.0);

    twist += sin(
        angle * 3.0 + radius * 9.0
    ) * deformation * 0.45;


    float petals = cos(
        twist * symmetry
    );

    float secondary = sin(
        twist * (symmetry + 3.0)
        - radius * 12.0
    );

    float shape =
        radius +
        petals * (0.16 + deformation * 0.22) +
        secondary * deformation * 0.12;


    float cellCoordinate =
        shape * (5.0 + par_a.x * 9.0);

    float cell = fract(cellCoordinate);

    float membrane = 1.0 - smoothstep(
        0.015,
        0.075,
        min(cell, 1.0 - cell)
    );


    float veins = abs(
        sin(
            twist * symmetry * 0.5 +
            radius * 17.0
        )
    );

    veins = 1.0 - smoothstep(
        0.025,
        0.12,
        veins
    );


    float cellIndex = floor(
        cellCoordinate
    );

    float hue =
        cellIndex * 0.13 +
        petals * 0.11 +
        radius * 0.2;

    vec3 pigment = spectrum(hue);


    float interior =
        0.5 + 0.5 * sin(
            radius * 21.0 -
            twist * 4.0
        );

    pigment *=
        0.3 + interior * 0.65;


    vec3 video = texture2D(
        vid,
        clamp(
            uv + vec2(
                petals,
                secondary
            ) * deformation * 0.035,
            0.0,
            1.0
        )
    ).rgb;

    pigment = mix(
        pigment,
        pigment * (0.35 + video * 1.3),
        par_a.z
    );


    pigment *= 1.0 -
        membrane * 0.92;


    pigment += veins *
        (1.0 - membrane) *
        vec3(0.7, 0.9, 1.0) *
        0.5;


    float silhouette =
        1.0 - smoothstep(
            0.78,
            0.98,
            shape
        );

    vec3 background = vec3(
        0.015,
        0.018,
        0.035
    );

    vec3 current = mix(
        background,
        pigment,
        silhouette
    );


    vec3 history = texture2D(
        bfr,
        uv
    ).rgb;

    vec3 result = mix(
        current,
        max(
            current,
            history * 0.965
        ),
        par_a.w
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
