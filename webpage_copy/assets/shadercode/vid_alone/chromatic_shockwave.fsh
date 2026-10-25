precision mediump float;

// CHROMATIC SHOCKWAVE
//
// Circular waves travel outward through vid, stretching and compressing the picture along their passing crests. Separated image channels fringe the ripples, making each ring appear to prise the picture apart.
//
// par_a.x - radial distortion strength.
// par_a.y - number and density of the circular waves.
// par_a.z - outward wave speed.
// par_a.w - channel separation along the distortion.

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

    float radius = length(p);


    float frequency =
        mix(8.0, 45.0, par_a.y);

    float phase =
        radius * frequency
        - time * mix(0.5, 8.0, par_a.z);

    float wave = sin(phase);


    vec2 direction =
        p / max(radius, 0.001);

    direction.x /= aspect;

    vec2 offset =
        direction * wave * par_a.x * 0.035;


    vec2 chroma =
        offset * par_a.w * 2.5;

    vec2 uvR = clamp(
        uv + offset + chroma,
        0.0, 1.0
    );

    vec2 uvG = clamp(
        uv + offset,
        0.0, 1.0
    );

    vec2 uvB = clamp(
        uv + offset - chroma,
        0.0, 1.0
    );


    float r = texture2D(vid, uvR).r;
    float g = texture2D(vid, uvG).g;
    float b = texture2D(vid, uvB).b;

    gl_FragColor = vec4(
        r, g, b, color.a
    );
}
