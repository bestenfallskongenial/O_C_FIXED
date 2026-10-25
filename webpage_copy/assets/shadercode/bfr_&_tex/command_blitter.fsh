precision mediump float;

// Command Blitter
//
// A source texture acts as a map of instructions for bfr. Its channels move retained pixels, attenuate their brightness, or erase them, turning the texture into a shifting arrangement of copied fragments and cleared openings.
//
// par_a.x - displacement distance commanded by the source texture.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Screen resolution

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];
uniform sampler2D bfr;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    /*
        tex[0] is a blit-command map, not the visible image.
        R/G encode source-address offset.
        B encodes copy/clear mode.
        A encodes operation strength.
    */
    vec4 cmd = texture2D(tex[0], uv);

    vec2 offset = (cmd.rg * 2.0 - 1.0) * mix(0.0, 0.50, par_a[0]);

    vec2 src_uv = fract(uv + offset);

    vec4 copied = texture2D(bfr, src_uv);

    float clearMask = step(0.75, cmd.b);
    float keepMask  = cmd.a;

    vec3 rgb = copied.rgb * keepMask;
    rgb = mix(rgb, vec3(0.0), clearMask);

    gl_FragColor = vec4(rgb, color.a);
}