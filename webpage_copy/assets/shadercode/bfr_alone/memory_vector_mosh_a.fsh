precision mediump float;

// Memory Vector Mosh
//
// The contents of bfr become instructions for moving its fragments. Sampled channels determine displacement and direction, creating shifting blocks that occasionally leap farther and exchange their channel order.
//
// par_a.x - update rate of block sizes and displacement bursts.
// par_a.y - upper range of block sizes.
// par_a.z - displacement strength.

uniform float time;
uniform vec2 tres;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;
uniform sampler2D bfr;

float hash(float x)
{
    return fract(sin(x * 91.3458) * 47453.5453);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    // local previous state
    vec3 here = texture2D(bfr, uv).rgb;

    float tick = floor(time * (2.0 + par_a.x * 20.0));

    // BUFFER CONTENT decides macroblock size
    float energy = dot(here, vec3(0.299, 0.587, 0.114));

    float bs =
        4.0 +
        floor(
            hash(tick + floor(energy * 31.0)) *
            mix(8.0, 80.0, par_a.y)
        );

    vec2 cell  = floor(gl_FragCoord.xy / bs);
    vec2 local = mod(gl_FragCoord.xy, bs);

    // sample centre of this dynamically-sized block
    vec2 probeUV =
        (cell * bs + bs * 0.5) / tres.xy;

    vec3 probe =
        texture2D(bfr, probeUV).rgb;

    // previous BLOCK COLOR becomes motion vector
    vec2 motion =
        (probe.rg * 2.0 - 1.0);

    // nonlinear amplification
    motion *=
        motion *
        mix(2.0, 70.0, par_a.z);

    // previous blue channel chooses direction reversal
    motion *=
        mix(-1.0, 1.0, step(0.5, probe.b));

    // occasional huge motion burst, but triggered by BUFFER
    float burst =
        step(
            0.82,
            fract(probe.r * 17.0 +
                  probe.g * 31.0 +
                  tick * 0.071)
        );

    motion *=
        mix(1.0, 5.0, burst);


    vec2 srcPixel =
        cell * bs +
        local -
        motion;

    vec2 srcUV =
        fract(srcPixel / tres.xy);

    vec3 mosh =
        texture2D(bfr, srcUV).rgb;


    float keep =
        fract(
            probe.r * 13.7 +
            probe.g * 7.3 +
            probe.b * 19.1 +
            time * 0.031
        );

    // sometimes mutate channels, sometimes preserve them
    vec3 mutate =
        mix(
            mosh,
            mosh.gbr,
            step(0.68, keep)
        );

    gl_FragColor =
        vec4(mutate, color.a);
}