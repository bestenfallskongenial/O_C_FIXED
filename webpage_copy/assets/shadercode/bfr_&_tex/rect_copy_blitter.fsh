precision mediump float;

// Rect Copy Blitter
//
// A moving rectangle copies fragments from elsewhere in bfr, leaving layered displacements across the fading canvas. A narrow vertical strip introduces fresh source imagery, while a bright outline traces the wandering copied region.
//
// par_a.x - speed of the rectangle’s horizontal travel and source-region movement.
// par_a.y - size of the copied rectangle.
// par_a.z - speed of the strip introducing fresh imagery.
// par_a.w - width of that strip.

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

float rectMask(vec2 p, vec2 lo, vec2 hi)
{
    return step(lo.x, p.x) *
           step(lo.y, p.y) *
           step(p.x, hi.x) *
           step(p.y, hi.y);
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    /*
        Default: copy previous framebuffer unchanged.
    */
    vec2 src_uv = uv;

    /*
        Moving blit rectangle.
        Destination rectangle moves across the screen.
        Its pixels are copied from another region of the previous frame.
    */
    float t = fract(time * mix(0.05, 0.60, par_a[0]));

    vec2 dst_size = vec2(mix(0.18, 0.65, par_a[1]),
                         mix(0.12, 0.45, par_a[1]));

    vec2 dst_pos = vec2(t * (1.0 - dst_size.x),
                        0.25 + 0.20 * sin(time * 0.37));

    vec2 dst_lo = dst_pos;
    vec2 dst_hi = dst_pos + dst_size;

    float m_dst = rectMask(uv, dst_lo, dst_hi);

    vec2 local = (uv - dst_lo) / dst_size;

    /*
        Source rectangle scrolls through the old framebuffer.
        This is the actual blit-copy source.
    */
    vec2 src_pos = vec2(fract(0.73 - t * 1.37),
                        fract(0.15 + t * 0.91));

    vec2 src_size = dst_size;

    vec2 rect_src_uv = src_pos + local * src_size;
    rect_src_uv = fract(rect_src_uv);

    src_uv = mix(src_uv, rect_src_uv, m_dst);

    vec4 old = texture2D(bfr, src_uv);

    /*
        Write head: inject fresh tex[0] only in a narrow vertical strip.
        Everything else is copied from old framebuffer memory.
    */
    float head_x = fract(time * mix(0.04, 0.50, par_a[2]) + seed.x);
    float head_w = mix(0.015, 0.12, par_a[3]);

    float head = 1.0 - step(head_w, abs(uv.x - head_x));

    vec4 fresh = texture2D(tex[0], uv);

    /*
        Slight decay so copied pixels do not become permanent.
    */
    vec3 rgb = old.rgb * 0.985;

    rgb = mix(rgb, fresh.rgb, head);

    /*
        Optional hard rectangle edge so the copied blit area is visibly mechanical.
    */
    float edge =
        m_dst *
        (1.0 - rectMask(uv, dst_lo + vec2(0.006, 0.006),
                            dst_hi - vec2(0.006, 0.006)));

    rgb = max(rgb, vec3(edge));

    gl_FragColor = vec4(rgb, color.a);
}