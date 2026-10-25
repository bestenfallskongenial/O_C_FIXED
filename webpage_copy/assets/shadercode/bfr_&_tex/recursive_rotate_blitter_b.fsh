precision mediump float;

// Rotating Canvas Blitter
//
// The retained canvas turns and changes scale through bfr, carrying earlier details into repeated trails. Portions leaving the frame fall into darkness, while a source texture can restore fresh imagery across the surface.
//
// par_a.x - rotation per feedback step; the midpoint stops rotation.
// par_a.y - feedback magnification.
// par_a.z - persistence of retained imagery.
// par_a.w - blends the transformed canvas toward the fresh source texture.

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

    vec2 p = uv - 0.5;

    /*
        Blitter-style source address transform:
        current destination pixel reads from a rotated/scaled
        address in the previous framebuffer.
    */

    float angle = (par_a[0] - 0.5) * 0.20;
    float s = sin(angle);
    float c = cos(angle);

    float zoom = mix(0.96, 1.04, par_a[1]);

    vec2 q;
    q.x = p.x * c - p.y * s;
    q.y = p.x * s + p.y * c;

    q /= zoom;

    vec2 src_uv = q + 0.5;

    vec4 old = texture2D(bfr, src_uv);

    float inside =
        step(0.0, src_uv.x) *
        step(0.0, src_uv.y) *
        step(src_uv.x, 1.0) *
        step(src_uv.y, 1.0);

    old *= inside;

    /*
        Persistent framebuffer decay.
    */

    float keep = mix(0.90, 0.999, par_a[2]);

    vec4 result = old * keep;

    /*
        tex[0] is only an initial/injection source.
        Set par_a[3] high only to seed or refresh the canvas.
        Set par_a[3] to 0 after seeding.
    */

    vec4 seed_img = texture2D(tex[0], uv);

    result.rgb = mix(result.rgb, seed_img.rgb, par_a[3]);

    gl_FragColor = vec4(result.rgb, color.a);
}