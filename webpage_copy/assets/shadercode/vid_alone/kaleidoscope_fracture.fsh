precision mediump float;

// KALEIDOSCOPE FRACTURE
//
// vid folds into rotating mirrored wedges, opening a radial ornament from fragments of the picture. Increasing the radial scale brings repeated details into the petals, while the original image can gradually return through the symmetry.
//
// par_a.x - number of mirrored sectors, from 2 to 12.
// par_a.y - rotation speed.
// par_a.z - radial sampling scale; higher values produce smaller, more repeated details.
// par_a.w - blends from the kaleidoscope to the original video.

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
#define TAU 6.28318530718

void main(void)
{
    vec2 uv = tcoord;


    float aspect = tres.x / max(tres.y, 1.0);

    vec2 p = uv - 0.5;
    p.x *= aspect;


    float sectors = floor(
        2.0 + par_a.x * 10.0
    );

    float sectorAngle = TAU / sectors;

    float angle = atan(p.y, p.x);

    angle += time * par_a.y * 0.6;

    angle = mod(angle, sectorAngle);

    angle = abs(
        angle - sectorAngle * 0.5
    );


    float radius = length(p);

    radius *= mix(
        1.0, 3.0, par_a.z
    );


    vec2 q = vec2(
        cos(angle),
        sin(angle)
    ) * radius;

    q.x /= aspect;

    vec2 sourceUV = fract(q + 0.5);


    vec3 kaleido = texture2D(
        vid,
        sourceUV
    ).rgb;


    vec3 original = texture2D(
        vid,
        uv
    ).rgb;

    vec3 result = mix(
        kaleido,
        original,
        par_a.w
    );

    gl_FragColor = vec4(
        result,
        color.a
    );
}
