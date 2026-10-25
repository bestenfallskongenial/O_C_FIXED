precision mediump float;

// CHROMATIC TIME GATE
//
// Brightness becomes a doorway between present and memory. Darker regions retain bfr, while brighter regions reveal fresh vid. A traveling threshold moves this boundary through the picture, leaving a shifting fringe along its edge.
//
// par_a.x - strength of the retained backbuffer and boundary treatment.
// par_a.y - brightness threshold separating fresh video from memory.
// par_a.z - sharpness of the boundary.
// par_a.w - speed of the threshold’s oscillation.

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


    vec3 live = texture2D(vid, uv).rgb;
    vec3 past = texture2D(bfr, uv).rgb;


    float lum = dot(
        live,
        vec3(0.299, 0.587, 0.114)
    );


    float sweep = sin(
        time * par_a.w * 2.0
    ) * 0.25;

    float threshold = clamp(
        par_a.y + sweep,
        0.0,
        1.0
    );

    float softness = mix(
        0.25,
        0.005,
        par_a.z
    );


    float gate = smoothstep(
        threshold - softness,
        threshold + softness,
        lum
    );


    float hold = par_a.x * (1.0 - gate);

    vec3 result = mix(
        live,
        past,
        hold
    );


    float boundary = 1.0 - abs(
        lum - threshold
    ) / max(softness * 2.0, 0.001);

    boundary = clamp(boundary, 0.0, 1.0);

    vec3 boundaryColor = vec3(
        live.b,
        live.r,
        live.g
    );

    result = mix(
        result,
        boundaryColor,
        boundary * par_a.x * 0.45
    );


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
