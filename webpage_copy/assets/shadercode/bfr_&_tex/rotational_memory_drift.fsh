precision mediump float;

// Rotational Memory Drift
//
// A gently warped source texture feeds a rotating memory field. bfr carries older details through small changes of scale and direction, drawing the picture into soft spiral traces that slowly gather and fade.
//
// par_a.x - feedback coordinate scale; higher values contract the retained image.
// par_a.y - backbuffer persistence.
// par_a.z - feedback rotation bias, with a small automatic oscillation.
// par_a.w - strength of fresh source imagery.
// par_b.x - wave distortion of the source texture.

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

#define PI 3.141592654

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    // previous frame coordinates

    vec2 p = uv - 0.5;

    float zoom = mix(0.985, 1.015, par_a.x);
    float angle = (par_a.z - 0.5) * 0.04;

    // time adds slow autonomous movement
    angle += sin(time * 0.17) * 0.006;

    float s = sin(angle);
    float c = cos(angle);

    p = mat2(c, -s,
             s,  c) * p;

    p *= zoom;

    vec2 feedbackUV = p + 0.5;

    // feedback

    vec3 old = texture2D(bfr, feedbackUV).rgb;

    float decay = mix(0.88, 0.995, par_a.y);

    // source texture

    vec2 srcUV = uv;

    float warp = par_b.x * 0.02;

    srcUV.x += sin(
        srcUV.y * 12.0 +
        time * 0.7
    ) * warp;

    srcUV.y += sin(
        srcUV.x * 10.0 -
        time * 0.5
    ) * warp;

    vec3 src = texture2D(tex[0], srcUV).rgb;

    // injection

    float injection = mix(0.002, 0.12, par_a.w);

    vec3 col =
        old * decay +
        src * injection;

    // keep runaway feedback bounded

    col = clamp(col, 0.0, 1.0);

    gl_FragColor = vec4(col, color.a);
}