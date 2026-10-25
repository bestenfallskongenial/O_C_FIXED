precision mediump float;

// PIXEL SORT
//
// Bright regions of vid borrow either the brightest or darkest sample from three horizontally spaced positions. Details spread sideways into stepped fragments, giving the picture a pulled and combed appearance.
//
// par_a.x - strength of the sample replacement.
// par_a.y - horizontal sampling distance.
// par_a.z - brightness threshold for applying the effect.
// par_a.w - blends between choosing the brightest and darkest samples.

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


float lum(vec3 c)
{
    return dot(c, vec3(0.299, 0.587, 0.114));
}

void main(void)
{
    vec2 uv = tcoord;


    float distancePx = mix(
        1.0, 64.0, par_a.y
    );

    float dx = distancePx /
        max(tres.x, 1.0);


    vec3 center = texture2D(
        vid, uv
    ).rgb;

    vec3 left = texture2D(
        vid,
        clamp(uv - vec2(dx, 0.0), 0.0, 1.0)
    ).rgb;

    vec3 right = texture2D(
        vid,
        clamp(uv + vec2(dx, 0.0), 0.0, 1.0)
    ).rgb;


    float lc = lum(center);
    float ll = lum(left);
    float lr = lum(right);


    vec3 bright = center;
    float maximum = lc;

    if (ll > maximum)
    {
        maximum = ll;
        bright = left;
    }

    if (lr > maximum)
    {
        bright = right;
    }


    vec3 dark = center;
    float minimum = lc;

    if (ll < minimum)
    {
        minimum = ll;
        dark = left;
    }

    if (lr < minimum)
    {
        dark = right;
    }


    vec3 sorted = mix(
        bright,
        dark,
        par_a.w
    );

    float threshold = smoothstep(
        par_a.z - 0.1,
        par_a.z + 0.1,
        lc
    );

    float amount =
        par_a.x * threshold;


    vec3 result = mix(
        center,
        sorted,
        amount
    );

    gl_FragColor = vec4(
        result,
        color.a
    );
}
