precision mediump float;

// Recursive Color Rupture
//
// Local edges in bfr redirect the retained image and amplify differences between neighboring pixels. Repeated tonal wrapping fractures smooth regions into sharp boundaries, building an increasingly restless field from the picture’s own small irregularities.
//
// par_a.x - displacement driven by local brightness gradients.
// par_a.y - rotation of that displacement according to the retained image.
// par_a.z - amplification of local differences.
// par_a.w - tonal gain before values wrap into repeated bands.

uniform float time;
uniform vec2 tres;
uniform vec4 par_a;
uniform vec4 par_b;
uniform sampler2D bfr;

void main()
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    vec2 px =
        1.0 / tres.xy;


    vec3 c =
        texture2D(bfr, uv).rgb;

    vec3 l =
        texture2D(
            bfr,
            fract(uv - vec2(px.x, 0.0))
        ).rgb;

    vec3 r =
        texture2D(
            bfr,
            fract(uv + vec2(px.x, 0.0))
        ).rgb;

    vec3 d =
        texture2D(
            bfr,
            fract(uv - vec2(0.0, px.y))
        ).rgb;

    vec3 u =
        texture2D(
            bfr,
            fract(uv + vec2(0.0, px.y))
        ).rgb;


    vec2 gradient;

    gradient.x =
        dot(r - l, vec3(0.333));

    gradient.y =
        dot(u - d, vec3(0.333));


    gradient *=
        mix(1.0, 90.0, par_a.x);

    // nonlinear rotation depending on current pixel
    float rot =
        (c.r - c.b) *
        3.14159 *
        par_a.y;

    float cs = cos(rot);
    float sn = sin(rot);

    gradient =
        mat2(
             cs, -sn,
             sn,  cs
        ) * gradient;


    vec2 fuv =
        fract(
            uv +
            gradient * px
        );

    vec3 history =
        texture2D(bfr, fuv).rgb;


    vec3 prediction =
        (l + r + u + d) * 0.25;

    vec3 error =
        c - prediction;


    history +=
        error *
        mix(0.0, 12.0, par_a.z);


    float drive =
        mix(1.0, 8.0, par_a.w);

    vec3 col =
        fract(
            abs(history) *
            drive
        );


    col +=
        sin(
            time * vec3(1.71, 2.13, 2.79) +
            c * 19.0
        )
        * 0.003;

    gl_FragColor =
        vec4(fract(col), color.a);
}