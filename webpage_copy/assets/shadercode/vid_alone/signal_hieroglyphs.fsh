precision mediump float;

// SIGNAL HIEROGLYPHS
//
// vid becomes a field of small symbols chosen by local brightness. Dots, bars, crosses, and lattices rebuild the moving picture as a changing script, with each cell carrying one fragment of its light.
//
// par_a.x - symbol grid density.
// par_a.y - brightness contrast used to choose symbols.
// par_a.z - blends the symbol coloring toward the sampled video.
// par_a.w - symbol stroke thickness.

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


float lineShape(vec2 p, vec2 a, vec2 b, float w)
{
    vec2 d = b - a;
    float t = clamp(dot(p - a, d) /
                    dot(d, d), 0.0, 1.0);

    float dist = length(p - a - d * t);

    return 1.0 - smoothstep(w, w + 0.025, dist);
}

void main(void)
{
    vec2 uv = tcoord;


    float rows = floor(mix(18.0, 100.0, par_a.x));
    float aspect = tres.x / max(tres.y, 1.0);

    vec2 grid = vec2(
        rows * aspect * 0.65,
        rows
    );

    vec2 cell = floor(uv * grid);
    vec2 p = fract(uv * grid);


    vec2 sampleUV = (cell + 0.5) / grid;

    vec3 source = texture2D(
        vid, sampleUV
    ).rgb;

    float lum = dot(
        source,
        vec3(0.299, 0.587, 0.114)
    );


    lum = clamp(
        (lum - 0.5) *
        mix(0.5, 2.5, par_a.y) + 0.5,
        0.0, 1.0
    );

    float symbol = floor(
        min(lum * 5.0, 4.0)
    );

    float w = mix(0.025, 0.105, par_a.w);

    float ink = 0.0;


    if (symbol < 1.0)
    {
        ink = 1.0 - smoothstep(
            w, w + 0.035,
            length(p - 0.5)
        );
    }


    else if (symbol < 2.0)
    {
        ink = lineShape(
            p, vec2(0.25, 0.5),
            vec2(0.75, 0.5), w
        );
    }


    else if (symbol < 3.0)
    {
        ink = max(
            lineShape(p,
                vec2(0.2, 0.5),
                vec2(0.8, 0.5), w),
            lineShape(p,
                vec2(0.5, 0.2),
                vec2(0.5, 0.8), w)
        );
    }


    else if (symbol < 4.0)
    {
        ink = max(
            lineShape(p,
                vec2(0.2), vec2(0.8), w),
            lineShape(p,
                vec2(0.2, 0.8),
                vec2(0.8, 0.2), w)
        );
    }


    else
    {
        ink = max(
            lineShape(p,
                vec2(0.35, 0.12),
                vec2(0.35, 0.88), w),
            lineShape(p,
                vec2(0.65, 0.12),
                vec2(0.65, 0.88), w)
        );

        ink = max(ink,
            lineShape(p,
                vec2(0.12, 0.35),
                vec2(0.88, 0.35), w)
        );

        ink = max(ink,
            lineShape(p,
                vec2(0.12, 0.65),
                vec2(0.88, 0.65), w)
        );
    }


    vec3 inkColor = mix(
        vec3(0.9, 1.0, 0.85),
        source,
        par_a.z
    );

    vec3 result = inkColor * ink;

    gl_FragColor = vec4(result, color.a);
}
