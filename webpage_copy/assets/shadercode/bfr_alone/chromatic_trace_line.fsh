precision mediump float;

// Chromatic Trace Line
//
// A narrow horizontal line travels across bfr, exchanging the red and blue channels wherever it passes. The rest of the retained picture stays in place, making the moving strip a small intervention in an otherwise persistent canvas.

varying vec2 tcoord;

uniform float time;
uniform vec2  tres;

uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;

uniform sampler2D tex[8];
uniform int tex_l;

uniform sampler2D bfr;

void main(void)
{
    vec2 uv =
        gl_FragCoord.xy / tres.xy;

    // THIS is the actual previous program/frame

    vec3 old =
        texture2D(bfr, uv).rgb;

    // moving diagnostic glitch line
    //
    // time is deliberately used here so we know
    // shader 4 itself is executing.

    float lineY =
        fract(time * 0.11);

    float line =
        1.0 -
        step(
            0.012,
            abs(uv.y - lineY)
        );

    // leave previous image untouched everywhere
    // except on moving diagnostic line

    vec3 diagnostic =
        old.bgr;

    vec3 col =
        mix(old, diagnostic, line);

    gl_FragColor =
        vec4(col, color.a);
}