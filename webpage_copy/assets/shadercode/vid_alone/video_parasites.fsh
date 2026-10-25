precision mediump float;

// VIDEO PARASITES
//
// Small circular lenses wander within a grid over vid. Each carries a rotating, magnified fragment from elsewhere in the picture, making the surface appear inhabited by drifting bubbles that borrow and rearrange its details.
//
// par_a.x - lens size.
// par_a.y - lens grid density.
// par_a.z - magnification and displacement of the imagery inside each lens.
// par_a.w - lens movement and rotation speed.

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
    return fract(
        sin(dot(p, vec2(127.1, 311.7)))
        * 43758.5453
    );
}

void main(void)
{
    vec2 uv = tcoord;


    float aspect = tres.x / max(tres.y, 1.0);

    vec2 grid = vec2(
        floor(mix(5.0, 24.0, par_a.y)) * aspect,
        floor(mix(5.0, 24.0, par_a.y))
    );

    vec2 cell = floor(uv * grid);

    vec2 local = fract(uv * grid);


    float id = hash(cell + seed.xy);

    float phase =
        time * par_a.w * 1.5
        + id * 6.2831853;


    vec2 center = vec2(
        0.5 + sin(phase) * 0.18,
        0.5 + cos(phase * 0.73) * 0.18
    );

    vec2 p = local - center;


    float radius =
        mix(0.0, 0.48, par_a.x)
        * (0.6 + id * 0.4);

    float distanceToCell = length(p);

    float mask = 1.0 - smoothstep(
        radius - 0.025,
        radius + 0.025,
        distanceToCell
    );


    float angle =
        phase * 0.35;

    float cs = cos(angle);
    float sn = sin(angle);

    vec2 rotated = vec2(
        p.x * cs - p.y * sn,
        p.x * sn + p.y * cs
    );

    float zoom = mix(
        1.0, 5.0, par_a.z
    );


    vec2 sourceUV =
        (cell + center + rotated / zoom)
        / grid;

    sourceUV +=
        (vec2(id, hash(cell.yx)) - 0.5)
        * par_a.z * 0.3;

    sourceUV = fract(sourceUV);


    vec3 original = texture2D(
        vid, uv
    ).rgb;

    vec3 parasite = texture2D(
        vid, sourceUV
    ).rgb;


    float rim = smoothstep(
        radius - 0.09,
        radius - 0.015,
        distanceToCell
    ) * mask;

    parasite *= 1.0 - rim * 0.35;


    vec3 result = mix(
        original,
        parasite,
        mask
    );

    gl_FragColor = vec4(
        result,
        color.a
    );
}
