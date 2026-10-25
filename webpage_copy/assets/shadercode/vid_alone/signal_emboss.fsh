precision mediump float;

// SIGNAL EMBOSS
//
// Brightness edges in vid become the slopes of a shallow relief. Directional lighting makes contours appear raised or recessed, turning moving details into an illuminated impression of the original picture.
//
// par_a.x - blends from the original video to the embossed treatment.
// par_a.y - apparent relief depth.
// par_a.z - lighting direction through a full turn.
// par_a.w - amount of original video color retained in the relief.

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


float luminance(vec3 c)
{
    return dot(c, vec3(0.299, 0.587, 0.114));
}

void main(void)
{
    vec2 uv = tcoord;

    vec2 px = vec2(
        1.0 / max(tres.x, 1.0),
        1.0 / max(tres.y, 1.0)
    );


    vec3 video = texture2D(vid, uv).rgb;


    float left = luminance(texture2D(
        vid, clamp(uv - vec2(px.x, 0.0), 0.0, 1.0)
    ).rgb);

    float right = luminance(texture2D(
        vid, clamp(uv + vec2(px.x, 0.0), 0.0, 1.0)
    ).rgb);

    float up = luminance(texture2D(
        vid, clamp(uv + vec2(0.0, px.y), 0.0, 1.0)
    ).rgb);

    float down = luminance(texture2D(
        vid, clamp(uv - vec2(0.0, px.y), 0.0, 1.0)
    ).rgb);


    float depth = mix(0.5, 8.0, par_a.y);

    vec3 normal = normalize(vec3(
        (left - right) * depth,
        (down - up) * depth,
        1.0
    ));


    float angle = par_a.z * 6.2831853;

    vec3 light = normalize(vec3(
        cos(angle),
        sin(angle),
        0.7
    ));

    float diffuse = max(
        dot(normal, light),
        0.0
    );


    float shade = 0.15 + diffuse * 0.85;

    vec3 material = mix(
        vec3(shade),
        video * shade * 1.5,
        par_a.w
    );


    vec3 result = mix(
        video,
        material,
        par_a.x
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
