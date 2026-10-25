precision mediump float;

// SHATTERED WINDOWS
//
// A rectangular grid breaks vid into independently wandering views. Each pane samples a nearby part of the picture, while narrow borders preserve the original image between the drifting fragments.
//
// par_a.x - displacement of the image inside each pane.
// par_a.y - grid density.
// par_a.z - movement speed.
// par_a.w - width of the borders showing the original video.

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

    float amount  = par_a.x;
    float density = par_a.y;
    float speed   = par_a.z;
    float gap     = par_a.w;


    vec2 grid = vec2(
        floor(mix(3.0, 24.0, density)),
        floor(mix(2.0, 18.0, density))
    );

    vec2 cell = floor(uv * grid);
    vec2 local = fract(uv * grid);


    float rndX = hash(cell + seed.xy);
    float rndY = hash(cell + vec2(37.2, 91.7));

    float phase = time * (0.2 + speed * 3.0);

    vec2 movement = vec2(
        sin(phase + rndX * 6.28318),
        cos(phase * 0.73 + rndY * 6.28318)
    );

    movement *= amount * 0.12;


    vec2 sourceUV = clamp(
        uv + movement,
        0.0,
        1.0
    );

    vec3 shifted = texture2D(
        vid,
        sourceUV
    ).rgb;


    float border = gap * 0.15;

    vec2 inside = step(
        vec2(border),
        local
    ) * step(
        local,
        vec2(1.0 - border)
    );

    float mask = inside.x * inside.y;


    vec3 original = texture2D(
        vid,
        uv
    ).rgb;

    vec3 result = mix(
        original,
        shifted,
        mask
    );

    gl_FragColor = vec4(
        result,
        color.a
    );
}
