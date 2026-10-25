precision mediump float;

// SOLARIZED CONTOURS
//
// The tonal structure of vid folds back on itself, bringing midtones forward while deep shadows and bright highlights recede. Traveling bands follow brightness contours, tracing moving terraces across the picture.
//
// par_a.x - blends from the original video to the solarized treatment.
// par_a.y - number of contour bands across the brightness range.
// par_a.z - strength of the contour-band treatment.
// par_a.w - speed of the bands moving through brightness levels.

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


    float lum = dot(
        video,
        vec3(0.299, 0.587, 0.114)
    );


    vec3 solar = 1.0 - abs(
        video * 2.0 - 1.0
    );


    float frequency = mix(
        3.0, 24.0, par_a.y
    );

    float phase =
        lum * frequency
        - time * par_a.w * 2.0;

    float bands = sin(phase * 6.2831853);

    bands = smoothstep(
        -0.15,
        0.15,
        bands
    );


    vec3 shifted = vec3(
        solar.r * (0.5 + bands * 0.5),
        solar.g * (1.0 - bands * 0.5),
        solar.b * (0.6 + bands * 0.4)
    );

    vec3 processed = mix(
        solar,
        shifted,
        par_a.z
    );


    vec3 result = mix(
        video,
        processed,
        par_a.x
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
