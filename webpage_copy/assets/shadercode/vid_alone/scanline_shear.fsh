precision mediump float;

// Scanline Shear
//
// Horizontal bands of vid jerk sideways and wrap across the picture’s edges. Fine row jitter unsettles the image between the larger tears, while separated channels and faint scanlines give it the fractured rhythm of a failing transmission.
//
// par_a.x - frequency of changes to the glitch pattern.
// par_a.y - number of horizontal bands.
// par_a.z - strength of band displacement and fine jitter; also increases how often bands shift.
// par_a.w - horizontal channel separation.

varying vec2 tcoord;

uniform float time;
uniform vec2 tres;

uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;

uniform sampler2D tex[8];

uniform sampler2D vid;

float randomValue(vec2 value)
{
    return fract(sin(dot(value, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    vec2 uv = tcoord;

    float timeStep =
        floor(time * mix(1.0, 20.0, par_a.x));

    float bandCount =
        mix(6.0, 48.0, par_a.y);

    float band =
        floor(uv.y * bandCount);

    float bandRandom =
        randomValue(vec2(band, timeStep) + seed.xy);

    float bandActive =
        step(1.0 - par_a.z * 0.35, bandRandom);

    float horizontalShift =
        (bandRandom - 0.5) *
        par_a.z *
        0.3 *
        bandActive;

    uv.x = fract(uv.x + horizontalShift);

    float fineNoise =
        randomValue(vec2(floor(uv.y * 160.0), timeStep) + seed.zw);

    uv.x = fract(
        uv.x +
        (fineNoise - 0.5) *
        par_a.z *
        0.008
    );

    float separation =
        par_a.w *
        0.025 *
        (0.5 + bandActive);

    float red = texture2D(
        vid,
        vec2(fract(uv.x + separation), uv.y)
    ).r;

    float green = texture2D(
        vid,
        uv
    ).g;

    float blue = texture2D(
        vid,
        vec2(fract(uv.x - separation), uv.y)
    ).b;

    float scanline =
        0.94 +
        0.06 * sin(uv.y * tres.y * 3.14159265);

    vec4 sampled = vec4(
        vec3(red, green, blue) * scanline,
        1.0
    );

    gl_FragColor = vec4(sampled.rgb, color.a);
}