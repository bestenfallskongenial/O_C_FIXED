precision mediump float;

// Recursive Rotate Blitter
//
// Successive copies of bfr rotate and change scale, drawing retained details into spirals or expanding trails. A source texture can continually replenish the canvas, balancing recognizable imagery against the accumulating transformation.
//
// par_a.x - rotation per feedback step; the midpoint stops rotation.
// par_a.y - feedback magnification.
// par_a.z - persistence of retained imagery.
// par_a.w - blends the transformed backbuffer toward the fresh source texture.

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
        par_a[0] = rotation per frame
        par_a[1] = zoom per frame
        par_a[2] = persistence
        par_a[3] = source injection
    */

    vec2 p = uv - 0.5;

    float angle = (par_a[0] - 0.5) * 0.40;
    float s = sin(angle);
    float c = cos(angle);

    float zoom = mix(0.94, 1.06, par_a[1]);

    vec2 q;
    q.x = p.x * c - p.y * s;
    q.y = p.x * s + p.y * c;

    q /= zoom;

    vec2 buv = q + 0.5;

    float inside =
        step(0.0, buv.x) *
        step(0.0, buv.y) *
        step(buv.x, 1.0) *
        step(buv.y, 1.0);

    vec4 old = texture2D(bfr, buv) * inside;

    float keep = mix(0.85, 0.999, par_a[2]);

    vec3 rgb = old.rgb * keep;

    /*
        tex[0] is not the main image.
        It is only injected when par_a[3] > 0.
        Set par_a[3] = 1.0 for first frame, then 0.0.
    */

    vec4 src = texture2D(tex[0], uv);

    rgb = mix(rgb, src.rgb, par_a[3]);

    gl_FragColor = vec4(rgb, color.a);
}