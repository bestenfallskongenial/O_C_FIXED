precision mediump float;

// Self Address Blitter
//
// bfr uses its own contents to choose where fragments move next. Rotation, scaling, small block jumps, and separated channel samples turn the retained picture into a self-rearranging mosaic, with fresh texture available to replenish its shifting surface.
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

float luma(vec3 c)
{
    return dot(c, vec3(0.299, 0.587, 0.114));
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    /*
        par_a[0] = rotation of old framebuffer memory
        par_a[1] = zoom of old framebuffer memory
        par_a[2] = persistence / decay
        par_a[3] = tex[0] seed / re-injection

        Proper test:
            frame 0:    par_a[3] = 1.0
            frame 1..N: par_a[3] = 0.0
    */

    vec2 px = 1.0 / tres.xy;

    /*
        Read previous framebuffer at current pixel.
        This is state, not decoration.
    */

    vec4 state0 = texture2D(bfr, uv);

    /*
        Use the old framebuffer itself as address-control.
        This is the part that tex[0] cannot replace after seeding:
        old pixels decide where old pixels are copied from.
    */

    vec2 self_vec = state0.rg * 2.0 - 1.0;

    float self_lum = luma(state0.rgb);

    /*
        Global blitter source-address transform:
        destination pixel reads old framebuffer from rotated/scaled source address.
    */

    vec2 p = uv - 0.5;

    float angle = (par_a[0] - 0.5) * 0.35;
    float s = sin(angle);
    float c = cos(angle);

    float zoom = mix(0.88, 1.14, par_a[1]);

    vec2 r;
    r.x = p.x * c - p.y * s;
    r.y = p.x * s + p.y * c;

    r /= zoom;

    vec2 src_uv = r + 0.5;

    /*
        Self-addressed framebuffer displacement.
        This makes it a recursive memory operation:

            frame[n](x,y) = frame[n-1]( transformed address derived from frame[n-1] )

        Not:

            frame[n](x,y) = effect(tex[0])
    */

    float push = 0.018 + self_lum * 0.085;

    src_uv += self_vec * push;

    /*
        Coarse memory-page snapping.
        Gives block/blitter behavior without using tex[0] as payload.
    */

    float block_count = 32.0;
    vec2 grid = vec2(block_count, floor(block_count * tres.y / tres.x));

    vec2 block_uv = floor(src_uv * grid) / grid;
    vec2 fine_uv  = fract(src_uv * grid) / grid;

    /*
        Neighbor-page selection from previous framebuffer state.
    */

    vec2 page_shift = vec2(0.0);

    if (state0.r > 0.75)
    {
        page_shift.x = 1.0 / grid.x;
    }
    else if (state0.g > 0.75)
    {
        page_shift.x = -1.0 / grid.x;
    }

    if (state0.b > 0.75)
    {
        page_shift.y = 1.0 / grid.y;
    }
    else if (self_lum < 0.20)
    {
        page_shift.y = -1.0 / grid.y;
    }

    src_uv = block_uv + fine_uv + page_shift;

    /*
        Wrap like framebuffer memory.
    */

    src_uv = fract(src_uv);

    /*
        Actual blit read:
        copy old framebuffer memory from computed source address.
    */

    vec4 old = texture2D(bfr, src_uv);

    /*
        Read neighboring old framebuffer pixels.
        This is still old framebuffer memory only.
    */

    vec4 old_x = texture2D(bfr, fract(src_uv + vec2(px.x, 0.0)));
    vec4 old_y = texture2D(bfr, fract(src_uv + vec2(0.0, px.y)));

    /*
        Blitter-like combine.
        Not source texture combine.
        This combines old framebuffer memory with old framebuffer memory.
    */

    vec3 mem = old.rgb;

    float edge = abs(luma(old_x.rgb) - luma(old_y.rgb));

    mem += edge * vec3(0.20, 0.12, 0.05);

    /*
        Channel memory skew.
        Each channel can come from a slightly different old framebuffer address.
    */

    vec2 skew = self_vec * px * 6.0;

    float rr = texture2D(bfr, fract(src_uv + skew)).r;
    float gg = texture2D(bfr, src_uv).g;
    float bb = texture2D(bfr, fract(src_uv - skew)).b;

    mem = mix(mem, vec3(rr, gg, bb), 0.35);

    /*
        Persistence.
    */

    float keep = mix(0.78, 0.998, par_a[2]);

    vec3 rgb = mem * keep;

    /*
        tex[0] is only seed / optional re-injection.
        Set par_a[3] to 0.0 after the first frame.
    */

    vec3 seed_img = texture2D(tex[0], uv).rgb;

    rgb = mix(rgb, seed_img, par_a[3]);

    gl_FragColor = vec4(rgb, color.a);
}