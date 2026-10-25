precision mediump float;

// SIGNAL GHOST
//
// Brightness in vid determines how far a displaced copy of bfr is sampled. Differences between shifted and unshifted history emerge as luminous ghost contours over the picture, while retained frames soften the present into an echo.
//
// par_a.x - brightness-dependent displacement of the ghost.
// par_a.y - strength of the ghost contours.
// par_a.z - backbuffer retention beneath the contours.

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


    vec3 video = texture2D(vid, uv).rgb;

    float brightness = dot(
        video,
        vec3(0.299, 0.587, 0.114)
    );


    float shift =
        par_a.x * 0.08 * brightness;

    vec2 ghostUV = clamp(
        uv + vec2(
            shift,
            shift * 0.25
        ),
        0.0,
        1.0
    );


    vec3 ghost = texture2D(
        bfr,
        ghostUV
    ).rgb;

    vec3 history = texture2D(
        bfr,
        uv
    ).rgb;


    vec3 difference = abs(
        ghost - history
    );

    vec3 spectral = vec3(
        difference.r * 0.6,
        difference.g,
        difference.b * 1.3
    );


    vec3 result = mix(
        video,
        history,
        par_a.z * 0.9
    );

    result +=
        spectral
        * par_a.y
        * 1.5;


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
