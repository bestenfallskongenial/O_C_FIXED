precision mediump float;

// IMPOSSIBLE ARCHITECTURE
//
// Repeated stepped tiles form a patchwork of ambiguous platforms and opposing stairways. Moving shadows slide across their faces while the grid shears, making neighboring structures seem to disagree about depth and direction.
//
// par_a.x - tile density.
// par_a.y - strength of the animated grid deformation.
// par_a.z - deformation and shadow movement speed.
// par_a.w - blends the material from pale stone toward copper.

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


float hash(vec2 p)
{
    return fract(sin(dot(
        p, vec2(127.1, 311.7)
    )) * 43758.5453);
}

void main(void)
{
    vec2 uv = tcoord;

    float aspect = tres.x / max(tres.y, 1.0);

    vec2 p = uv - 0.5;
    p.x *= aspect;


    float t = time * par_a.z * 0.4;

    p.x += p.y * (
        0.35 + par_a.y * sin(t)
    );

    p.y += p.x * par_a.y * 0.2 *
        cos(t * 0.7);


    float density = mix(
        3.0, 18.0, par_a.x
    );

    vec2 q = p * density;

    vec2 id = floor(q);
    vec2 f = fract(q);


    float rnd = hash(id + seed.xy);

    if (rnd > 0.5)
    {
        f = f.yx;
    }


    float stair = floor(
        (f.x + f.y) * 4.0
    ) / 4.0;

    float stepEdge = 1.0 - smoothstep(
        0.015,
        0.055,
        abs(fract((f.x + f.y) * 4.0) - 0.5)
    );


    float planeA = step(
        f.y,
        f.x * 0.65 + 0.15
    );

    float planeB = step(
        f.x,
        1.0 - f.y * 0.65
    );

    float face = planeA * planeB;


    float illumination =
        0.25 + stair * 0.65;

    illumination +=
        face * 0.3;

    illumination -=
        stepEdge * 0.4;


    float shadow = step(
        f.y,
        fract(f.x + t * (0.2 + rnd * 0.3))
    );

    illumination *= mix(
        0.55,
        1.0,
        shadow
    );


    vec3 stone = vec3(
        0.75,
        0.80,
        0.85
    );

    vec3 copper = vec3(
        0.95,
        0.48,
        0.18
    );

    vec3 material = mix(
        stone,
        copper,
        par_a.w
    );


    float alternate = mod(
        id.x + id.y,
        2.0
    );

    material *= mix(
        0.6,
        1.2,
        alternate
    );


    vec3 result =
        material * illumination;

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
