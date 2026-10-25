precision mediump float;

// MEMORY GARDEN
//
// Small seeds spread through bfr, borrowing material from neighboring pixels to grow a restless, branching texture. vid supplies brightness that encourages germination and can lend its own appearance to the emerging growth.
//
// par_a.x - strength of growth and new seed deposits.
// par_a.y - distance over which neighboring growth spreads.
// par_a.z - decay rate; higher values make the growth fade faster.
// par_a.w - influence of video brightness and color on new growth.

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
    return fract(sin(dot(
        p, vec2(127.1, 311.7)
    )) * 43758.5453);
}

void main(void)
{
    vec2 uv = tcoord;

    float growth = par_a.x;
    float branch = par_a.y;
    float decay  = par_a.z;
    float inputMix = par_a.w;

    vec2 px = 1.0 / max(tres, vec2(1.0));


    vec3 old = texture2D(bfr, uv).rgb;


    float angle =
        hash(floor(uv * tres * 0.125))
        * 6.2831853
        + time * 0.15;

    vec2 direction = vec2(
        cos(angle),
        sin(angle)
    );

    vec2 offset = direction * px *
        mix(1.0, 8.0, branch);

    vec3 neighborA = texture2D(
        bfr,
        clamp(uv + offset, 0.0, 1.0)
    ).rgb;

    vec3 neighborB = texture2D(
        bfr,
        clamp(uv - offset, 0.0, 1.0)
    ).rgb;


    vec3 video = texture2D(vid, uv).rgb;

    float nutrient = dot(
        video, vec3(0.299, 0.587, 0.114)
    );


    vec3 spread = max(
        neighborA,
        neighborB
    );

    float noise = hash(
        floor(uv * tres * 0.5)
        + floor(time * 8.0)
    );

    float germination = smoothstep(
        0.65, 0.95,
        noise + nutrient * inputMix * 0.5
    );


    vec3 growthColor = vec3(
        spread.g * 0.85,
        spread.b * 0.95,
        spread.r * 1.05
    );

    vec3 result = mix(
        old,
        growthColor,
        growth * 0.35
    );


    vec3 seedColor = mix(
        vec3(0.15, 0.75, 0.35),
        video,
        inputMix
    );

    result = max(
        result,
        seedColor * germination * growth
    );


    result *= 1.0 - decay * 0.035;


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
