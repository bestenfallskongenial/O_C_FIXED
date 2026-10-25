precision mediump float;

// Memory Blitter Rotor
//
// bfr rotates and scales while selected strips slip sideways and rectangular fragments jump into neighboring positions. A source texture can replenish the disrupted picture, balancing fresh imagery against the rotor’s accumulating dislocations.
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

float hash21(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    /*
        par_a[0] = rotation amount
        par_a[1] = zoom amount
        par_a[2] = persistence / decay
        par_a[3] = tex[0] seed / re-injection

        Normal operation after seeding:
            par_a[3] = 0.0

        Then visible image comes only from bfr.
    */

    vec2 p = uv - 0.5;

    /*
        Global bfr roto-zoom blit.
        Destination pixel reads previous-frame memory from transformed source address.
    */

    float angle = (par_a[0] - 0.5) * 0.28;
    float s = sin(angle);
    float c = cos(angle);

    float zoom = mix(0.90, 1.12, par_a[1]);

    vec2 r;
    r.x = p.x * c - p.y * s;
    r.y = p.x * s + p.y * c;

    r /= zoom;

    vec2 src_uv = r + 0.5;

    /*
        Blitter/glitch memory behavior:
        scanline/block address displacement is applied to BACKBUFFER source address.
    */

    float bands = 48.0;
    float band  = floor(uv.y * bands);

    float h0 = hash21(vec2(band, floor(time * 8.0)));
    float h1 = hash21(vec2(band + 19.0, floor(time * 3.0)));

    float strip_gate = step(0.72, h0);

    float strip_shift = (h1 - 0.5) * 0.22 * strip_gate;
    src_uv.x += strip_shift;

    /*
        Coarse block copy disturbance.
        This emulates old framebuffer chunks being copied from nearby memory.
    */

    vec2 grid = vec2(16.0, 12.0);
    vec2 cell = floor(uv * grid);

    float bh = hash21(cell + floor(time * 4.0));

    vec2 block_shift = vec2(0.0);

    if (bh < 0.20)
    {
        block_shift = vec2( 1.0,  0.0) / grid;
    }
    else if (bh < 0.40)
    {
        block_shift = vec2(-1.0,  0.0) / grid;
    }
    else if (bh < 0.60)
    {
        block_shift = vec2( 0.0,  1.0) / grid;
    }
    else if (bh < 0.80)
    {
        block_shift = vec2( 0.0, -1.0) / grid;
    }

    float block_gate = step(0.82, hash21(cell + vec2(floor(time * 2.0), 91.0)));

    src_uv += block_shift * block_gate;

    /*
        Wrap source coordinates.
        This gives framebuffer-memory recirculation instead of black borders.
    */

    src_uv = fract(src_uv);

    vec4 old = texture2D(bfr, src_uv);

    /*
        Persistence.
    */

    float keep = mix(0.82, 0.997, par_a[2]);

    vec3 rgb = old.rgb * keep;

    /*
        tex[0] is not the normal image source.
        It is only seed / re-injection.

        frame 0:
            par_a[3] = 1.0

        feedback/blitter mode:
            par_a[3] = 0.0

        occasional refresh:
            par_a[3] = 0.02 .. 0.10
    */

    vec3 src = texture2D(tex[0], uv).rgb;

    rgb = mix(rgb, src, par_a[3]);

    gl_FragColor = vec4(rgb, color.a);
}