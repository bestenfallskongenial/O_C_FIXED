precision mediump float;

// EDGE ENGRAVER
//
// vid becomes an engraving of dark contours and slanting strokes. The hatching gathers in shadowed regions and bends with the picture’s brightness, while edges cut through it like marks pressed into a plate.
//
// par_a.x - blends from the original video to the engraving.
// par_a.y - edge sensitivity.
// par_a.z - hatching density.
// par_a.w - amount of original video color retained in the engraving.

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


float gray(vec3 c)
{
    return dot(c, vec3(0.299, 0.587, 0.114));
}

void main(void)
{
    vec2 uv = tcoord;


    vec2 px = 1.0 / max(tres, vec2(1.0));

    vec3 video = texture2D(vid, uv).rgb;

    float l = gray(texture2D(
        vid, clamp(uv - vec2(px.x, 0.0), 0.0, 1.0)
    ).rgb);

    float r = gray(texture2D(
        vid, clamp(uv + vec2(px.x, 0.0), 0.0, 1.0)
    ).rgb);

    float u = gray(texture2D(
        vid, clamp(uv + vec2(0.0, px.y), 0.0, 1.0)
    ).rgb);

    float d = gray(texture2D(
        vid, clamp(uv - vec2(0.0, px.y), 0.0, 1.0)
    ).rgb);


    vec2 gradient = vec2(r - l, u - d);

    float edge = length(gradient);

    float sensitivity = mix(
        0.25, 5.0, par_a.y
    );

    edge = clamp(
        edge * sensitivity * 3.0,
        0.0,
        1.0
    );


    float lum = gray(video);

    float frequency = mix(
        40.0, 220.0, par_a.z
    );

    float strokes = sin(
        (uv.x + uv.y * 0.55) * frequency
        + lum * 8.0
    );

    float ink = smoothstep(
        0.0, 0.35,
        strokes
    );


    float shading = mix(
        1.0,
        ink,
        1.0 - lum
    );

    shading *= 1.0 - edge * 0.85;

    vec3 engraving = vec3(shading);


    engraving = mix(
        engraving,
        engraving * video,
        par_a.w
    );


    vec3 result = mix(
        video,
        engraving,
        par_a.x
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
