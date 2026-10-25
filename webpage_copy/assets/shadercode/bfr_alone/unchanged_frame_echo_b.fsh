precision mediump float;

// Held Frame Canvas
//
// bfr returns directly to the screen, holding the existing picture in place. The shader preserves the frame without moving, reshaping, or adding imagery.

varying vec2 tcoord;

uniform float time;
uniform vec2 tres;

uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;

uniform sampler2D bfr;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    vec4 bfr = texture2D(bfr, uv);

    gl_FragColor = vec4(bfr.rgb, color.a);
}