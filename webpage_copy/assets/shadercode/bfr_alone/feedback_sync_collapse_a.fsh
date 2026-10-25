precision mediump float;

// Feedback Sync Collapse
//
// Rows of bfr control their own misalignment through a moving sampling probe. Horizontal compression, sideways jumps, reversals, and vertical offsets accumulate into a picture that continually loses and rebuilds its internal alignment.
//
// par_a.x - strength of row-dependent horizontal shifts.
// par_a.y - influence of the sampled signal on horizontal scaling.
// par_a.z - vertical displacement strength.
// par_a.w - strength of the fine horizontal tremor.

uniform float time;
uniform vec2 tres;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;
uniform sampler2D bfr;

void main()
{
    vec2 uv = gl_FragCoord.xy / tres.xy;


    float head =
        fract(
            0.173 +
            sin(time * 0.117) * 0.143
        );

    // EACH scanline obtains control values from its
    // own previous contents
    vec3 ctrl =
        texture2D(
            bfr,
            vec2(head, uv.y)
        ).rgb;


    float shift =
          (ctrl.r - ctrl.b)
        * mix(0.0, 0.9, par_a.x);

    float stretch =
        0.2 +
        ctrl.g *
        mix(0.5, 4.0, par_a.y);


    float roll =
        (ctrl.r + ctrl.g + ctrl.b - 1.5)
        * mix(0.0, 0.20, par_a.z);


    vec2 p =
        uv - 0.5;

    p.x *= stretch;

    p.x += shift;
    p.y += roll;


    float reverse =
        step(
            ctrl.r + ctrl.b,
            ctrl.g * 1.7
        );

    p.x =
        mix(
            p.x,
            -p.x,
            reverse
        );


    p.x +=
        sin(
            p.y * tres.y * 0.071 +
            time * 5.7 +
            ctrl.b * 40.0
        )
        *
        ctrl.r
        *
        0.025
        *
        par_a.w;

    vec2 srcUV =
        fract(p + 0.5);

    vec3 col =
        texture2D(bfr, srcUV).rgb;


    float overload =
        step(1.35, ctrl.r + ctrl.g + ctrl.b);

    col =
        mix(
            col,
            col.brg * 1.15,
            overload
        );

    gl_FragColor =
        vec4(clamp(col, 0.0, 1.0), color.a);
}