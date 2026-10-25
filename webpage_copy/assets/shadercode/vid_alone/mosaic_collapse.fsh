precision mediump float;

// MOSAIC COLLAPSE
//
// vid divides into rectangular tiles that shrink in a traveling wave. Each tile carries its own piece of the picture inward, exposing the original image beneath like a mosaic repeatedly opening its joints.
//
// par_a.x - tile contraction strength.
// par_a.y - grid density.
// par_a.z - speed of the contraction wave.
// par_a.w - darkening of the image exposed between tiles.

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

    float amount  = par_a.x;
    float density = par_a.y;
    float speed   = par_a.z;
    float darken  = par_a.w;


    vec2 grid = vec2(
        floor(mix(4.0, 32.0, density)),
        floor(mix(3.0, 24.0, density))
    );

    vec2 cell = floor(uv * grid);
    vec2 local = fract(uv * grid) - 0.5;


    float phase =
        cell.x * 0.41
        + cell.y * 0.57
        - time * mix(0.3, 5.0, speed);

    float wave = sin(phase) * 0.5 + 0.5;

    float collapse =
        amount * wave * 0.85;


    float tileSize = 1.0 - collapse;

    vec2 sourceUV =
        (cell + local / max(tileSize, 0.15) + 0.5)
        / grid;


    vec2 inside = step(
        abs(local),
        vec2(tileSize * 0.5)
    );

    float mask = inside.x * inside.y;


    vec3 tile = texture2D(
        vid,
        clamp(sourceUV, 0.0, 1.0)
    ).rgb;

    vec3 background = texture2D(
        vid,
        uv
    ).rgb;


    background *= 1.0 - darken;

    vec3 result = mix(
        background,
        tile,
        mask
    );

    gl_FragColor = vec4(
        result,
        color.a
    );
}
