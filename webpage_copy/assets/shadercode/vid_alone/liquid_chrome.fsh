precision mediump float;

// LIQUID CHROME
//
// Moving reflection bands follow the brightness contours of vid, turning the picture into a polished relief. Highlights slide across recognizable forms, narrowing into sharp glints as the surface becomes more metallic.
//
// par_a.x - blends from the original video to the chrome treatment.
// par_a.y - contrast of the metallic shading.
// par_a.z - sharpness of the reflection bands.
// par_a.w - reflection movement speed.

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


    float contrast = mix(
        1.0, 3.5, par_a.y
    );

    float metallic = clamp(
        (lum - 0.5) * contrast + 0.5,
        0.0, 1.0
    );


    float wave = sin(
        uv.y * 18.0
        + lum * 8.0
        - time * par_a.w * 3.0
    );

    float reflection = wave * 0.5 + 0.5;

    float sharpness = mix(
        1.0, 12.0, par_a.z
    );

    reflection = pow(
        reflection,
        sharpness
    );


    vec3 graphite = vec3(
        0.035, 0.045, 0.065
    );

    vec3 silver = vec3(
        0.75, 0.85, 1.0
    );

    vec3 chrome = mix(
        graphite,
        silver,
        metallic
    );

    chrome += reflection
        * vec3(0.5, 0.65, 0.9)
        * 0.6;


    vec3 result = mix(
        video,
        chrome,
        par_a.x
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
