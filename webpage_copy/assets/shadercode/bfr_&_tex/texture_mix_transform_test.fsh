precision mediump float;

// Texture Mix Transform Test
//
// A rotated and magnified source texture overlays bfr, allowing a fresh picture to emerge through the retained canvas. The transformed image ends at its own boundaries, leaving the surrounding frame to the fading memory beneath it.
//
// par_a.x - source-texture magnification.
// par_a.y - source-texture rotation through a full turn.
// par_a.z - blends from the retained backbuffer to the transformed source texture.
// par_a.w - brightness of the backbuffer contribution.

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

    vec4 bfr = texture2D(bfr, uv);

    vec2 p = uv - 0.5;

    float zoom = mix(0.75, 2.50, par_a.x);
    p /= zoom;

    float ang = (par_a.y - 0.5) * 6.2831853;
    float s = sin(ang);
    float c = cos(ang);

    vec2 r;
    r.x = p.x * c - p.y * s;
    r.y = p.x * s + p.y * c;

    vec2 tuv = r + 0.5;

    vec4 img = texture2D(tex[0], tuv);

    float inside =
        step(0.0, tuv.x) *
        step(0.0, tuv.y) *
        step(tuv.x, 1.0) *
        step(tuv.y, 1.0);

    img.rgb *= inside;
    img.a   *= inside;

    float texMix = par_a.z;
    float fbMix  = par_a.w;

    vec3 rgb = mix(bfr.rgb * fbMix, img.rgb, texMix);

    gl_FragColor = vec4(rgb, color.a);
}