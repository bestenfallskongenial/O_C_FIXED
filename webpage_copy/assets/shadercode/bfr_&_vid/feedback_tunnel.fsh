precision mediump float;

// FEEDBACK TUNNEL
//
// Successive copies of the picture contract and turn toward the center, building a receding tunnel from bfr. A wandering circular opening admits fresh vid, feeding new fragments into the procession.
//
// par_a.x - contraction of each feedback copy.
// par_a.y - rotation between successive feedback copies.
// par_a.z - backbuffer retention.
// par_a.w - size of the moving opening that reveals fresh video.

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

    float depth    = par_a.x;
    float rotation = par_a.y;
    float memory   = par_a.z;
    float opening  = par_a.w;


    vec2 p = uv - 0.5;

    float angle =
        rotation * 0.035;

    float c = cos(angle);
    float s = sin(angle);

    p = mat2(
        c, -s,
        s,  c
    ) * p;

    p *= 1.0 + depth * 0.06;

    vec2 historyUV = clamp(
        p + 0.5,
        0.0,
        1.0
    );


    vec3 history = texture2D(
        bfr,
        historyUV
    ).rgb;


    vec3 video = texture2D(
        vid,
        uv
    ).rgb;


    vec2 center = vec2(
        0.5 + sin(time * 0.37) * 0.2,
        0.5 + cos(time * 0.29) * 0.2
    );

    vec2 d = uv - center;

    d.x *= tres.x / max(tres.y, 1.0);

    float radius = length(d);

    float aperture = mix(
        0.03,
        0.38,
        opening
    );

    float injection = 1.0 - smoothstep(
        aperture,
        aperture + 0.025,
        radius
    );


    float retention =
        memory * 0.985;

    vec3 result = mix(
        video,
        history,
        retention
    );

    result = mix(
        result,
        video,
        injection
    );


    result *= 0.995;

    gl_FragColor = vec4(
        result,
        color.a
    );
}
