precision mediump float;

// State Blitter Field
//
// Sparse seeds appear in dark regions and circulate through bfr. The retained image steers itself, while a source texture supplies edge forces and block-shift instructions, drawing the seeds into fractured currents and separated trails.
//
// par_a.x - rotation per feedback step; the midpoint stops rotation.
// par_a.y - feedback magnification.
// par_a.z - persistence of the circulating imagery.
// par_a.w - strength of texture-driven displacement, block shifts, and channel separation.

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

float lum(vec3 c)
{
    return dot(c, vec3(0.299, 0.587, 0.114));
}

float hash21(vec2 p)
{
    p = fract(p * vec2(123.34, 345.45));
    p += dot(p, p + 34.345);
    return fract(p.x * p.y);
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;
    vec2 px = 1.0 / tres.xy;

    /*
        par_a[0] = rotation
        par_a[1] = zoom
        par_a[2] = persistence
        par_a[3] = control strength

        Visible RGB source:
            bfr only, plus procedural bootstrap when memory is empty.

        tex[0] role:
            control field only.
            Its RGB is never copied to output.
    */

    vec4 here = texture2D(bfr, uv);

    /*
        Detect empty/black previous frame.
        Needed because without any nonzero state, recursive feedback stays black.
    */

    float has_state = step(0.002, lum(here.rgb));

    /*
        Procedural bootstrap.
        This is not tex[0].
        It creates initial framebuffer state without a C++ seed pass.
    */

    vec2 gp = floor(uv * vec2(64.0, 64.0));
    float boot = hash21(gp + seed.xy * 97.0);
    vec3 boot_rgb = vec3(
        step(0.985, boot),
        step(0.990, hash21(gp + 17.0)),
        step(0.995, hash21(gp + 43.0))
    );

    /*
        tex[0] as control map only.
        Use local contrast to steer where OLD FRAMEBUFFER pixels are copied from.
    */

    vec3 ctl = texture2D(tex[0], uv).rgb;

    float cx1 = lum(texture2D(tex[0], uv + vec2(px.x, 0.0)).rgb);
    float cx0 = lum(texture2D(tex[0], uv - vec2(px.x, 0.0)).rgb);
    float cy1 = lum(texture2D(tex[0], uv + vec2(0.0, px.y)).rgb);
    float cy0 = lum(texture2D(tex[0], uv - vec2(0.0, px.y)).rgb);

    vec2 control_vec = vec2(cx1 - cx0, cy1 - cy0);

    /*
        Backbuffer source-address transform.
        This is the blitter operation:
            destination pixel reads old framebuffer at computed source address.
    */

    vec2 p = uv - 0.5;

    float rot = (par_a[0] - 0.5) * 0.70;
    float s = sin(rot);
    float c = cos(rot);

    float zoom = mix(0.78, 1.28, par_a[1]);

    vec2 q;
    q.x = p.x * c - p.y * s;
    q.y = p.x * s + p.y * c;
    q /= zoom;

    vec2 src_uv = q + 0.5;

    /*
        Self-addressing from the old framebuffer itself.
    */

    vec2 self_vec = here.rg * 2.0 - 1.0;
    float self_e = lum(here.rgb);

    src_uv += self_vec * self_e * 0.18;
    src_uv += control_vec * mix(0.0, 0.30, par_a[3]);

    /*
        Block/page blitter addressing.
        Still addresses only previous framebuffer memory.
    */

    vec2 grid = vec2(32.0, floor(32.0 * tres.y / tres.x));
    vec2 cell = floor(uv * grid);
    vec2 cell_uv = (cell + 0.5) / grid;

    float page_ctl = lum(texture2D(tex[0], cell_uv).rgb);
    float page_sel = floor(page_ctl * 4.0);

    vec2 page = vec2(0.0);
    if (page_sel < 1.0)
    {
        page = vec2(1.0, 0.0);
    }
    else if (page_sel < 2.0)
    {
        page = vec2(-1.0, 0.0);
    }
    else if (page_sel < 3.0)
    {
        page = vec2(0.0, 1.0);
    }
    else
    {
        page = vec2(0.0, -1.0);
    }

    src_uv += page / grid * par_a[3];

    src_uv = fract(src_uv);

    /*
        All visible sampled color comes from previous framebuffer memory.
    */

    vec4 old0 = texture2D(bfr, src_uv);

    vec2 skew = (old0.rg * 2.0 - 1.0) * px * mix(0.0, 24.0, par_a[3]);

    float r = texture2D(bfr, fract(src_uv + skew)).r;
    float g = texture2D(bfr, src_uv).g;
    float b = texture2D(bfr, fract(src_uv - skew)).b;

    vec3 mem = vec3(r, g, b);

    float keep = mix(0.80, 0.999, par_a[2]);

    vec3 rgb = mem * keep;

    /*
        Bootstrap only while previous frame is effectively empty.
        After state exists, this contributes zero.
    */

    rgb = mix(boot_rgb, rgb, has_state);

    gl_FragColor = vec4(rgb, color.a);
}