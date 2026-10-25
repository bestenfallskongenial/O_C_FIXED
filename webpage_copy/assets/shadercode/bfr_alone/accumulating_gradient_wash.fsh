precision mediump float;

// Accumulating Gradient Wash
//
// A smooth positional wash is added to bfr on every frame. The retained picture steadily brightens, with different parts of the frame accumulating at different rates until its details dissolve into saturation.

varying vec2 tcoord;

uniform float time;
uniform vec2 tres;
uniform vec4 color;

uniform sampler2D bfr;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    vec4 bfr = texture2D(bfr, uv);

    vec3 test = bfr.rgb + vec3(uv.x * 0.25, uv.y * 0.25, 0.25);

    gl_FragColor = vec4(test, color.a);
}