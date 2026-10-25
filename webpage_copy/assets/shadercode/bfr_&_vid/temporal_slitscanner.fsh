precision mediump float;

// TEMPORAL SLITSCANNER
//
// Horizontal strips of vid jump sideways on independent clocks. bfr holds and drifts selected bands, while a traveling rupture pulls distorted history across them, stitching fresh movement and lingering fragments into an unsettled temporal collage.
//
// par_a.x - strip displacement, internal shear, and rupture strength.
// par_a.y - number of horizontal strips.
// par_a.z - retention of historical strips and strength of their edge echoes.
// par_a.w - update speed, rupture travel speed, and additional history jitter.

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
    return fract(
        sin(dot(p, vec2(127.1, 311.7)))
        * 43758.5453
    );
}

void main(void)
{
    vec2 uv = tcoord;

    float displacement = par_a.x;
    float density      = par_a.y;
    float persistence  = par_a.z;
    float instability  = par_a.w;


    float bands = floor(
        mix(5.0, 180.0, density)
    );

    float band = floor(uv.y * bands);

    float phase = time * mix(
        0.2, 12.0, instability
    );

    float id = hash(
        vec2(band, seed.x)
    );


    float bandClock = floor(
        phase * mix(0.2, 3.0, id)
    );

    float jump = hash(
        vec2(band, bandClock)
    );


    float shift =
        (jump * 2.0 - 1.0)
        * displacement
        * 0.85;

    vec2 videoUV = uv;

    videoUV.x = fract(
        videoUV.x + shift
    );


    float localY = fract(uv.y * bands);

    float shear =
        sin(
            localY * 6.2831853
            + phase
            + id * 13.0
        )
        * displacement
        * 0.035;

    videoUV.x = fract(
        videoUV.x + shear
    );


    vec3 live =
        texture2D(vid, videoUV).rgb;


    float drift =
        (id - 0.5)
        * displacement
        * 0.035;

    vec2 oldUV = uv;

    oldUV.y = fract(
        oldUV.y + drift
    );

    oldUV.x = fract(
        oldUV.x
        + shift * 0.035
        + sin(phase + band * 0.7)
        * instability * 0.005
    );

    vec3 old =
        texture2D(bfr, oldUV).rgb;


    float freeze = hash(
        vec2(
            band,
            floor(phase * 0.35)
        )
    );

    float memory = persistence *
        smoothstep(
            0.15,
            0.85,
            freeze
        );

    vec3 result = mix(
        live,
        old,
        memory
    );


    float ruptureCenter = fract(
        time * mix(0.03, 0.75, instability)
    );

    float distanceToRupture = abs(
        uv.y - ruptureCenter
    );

    distanceToRupture = min(
        distanceToRupture,
        1.0 - distanceToRupture
    );

    float ruptureWidth = mix(
        0.005,
        0.14,
        displacement
    );

    float rupture = 1.0 -
        smoothstep(
            0.0,
            ruptureWidth,
            distanceToRupture
        );


    vec2 ruptureUV = fract(
        uv + vec2(
            sin(uv.y * 42.0 + phase)
            * displacement * 0.2,
            cos(uv.x * 16.0 - phase)
            * displacement * 0.04
        )
    );

    vec3 displacedPast =
        texture2D(bfr, ruptureUV).rgb;

    result = mix(
        result,
        displacedPast,
        rupture * displacement
    );


    vec2 pixel = 1.0 / max(
        tres,
        vec2(1.0)
    );

    vec3 echo = texture2D(
        bfr,
        fract(
            oldUV + vec2(
                pixel.x * (2.0 + displacement * 35.0),
                0.0
            )
        )
    ).rgb;

    float echoEdge = length(
        old - echo
    );

    result +=
        echoEdge
        * vec3(0.35, 0.08, 0.7)
        * displacement
        * persistence
        * 0.4;


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
