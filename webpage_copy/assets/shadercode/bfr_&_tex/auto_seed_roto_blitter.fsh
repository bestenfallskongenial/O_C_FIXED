precision mediump float;

// Auto Seed Roto Blit
//
// A source texture fills dark regions while existing imagery turns and scales through bfr. The retained picture also steers its own displacement, allowing seeded details to curl into a continuously reshaping feedback pattern.
//
// par_a.x - rotation per feedback step; the midpoint stops rotation.
// par_a.y - feedback magnification.
// par_a.z - persistence of retained imagery.
// par_a.w - displacement driven by the backbuffer’s color and brightness.

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

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    /*
        par_a[0] = rotation
        par_a[1] = zoom
        par_a[2] = persistence
        par_a[3] = self-displacement strength

        tex[0] is used only if the previous frame is effectively empty.
    */

    vec4 prev_here = texture2D(bfr, uv);

    float state = step(0.004, lum(prev_here.rgb));

    vec2 p = uv - 0.5;

    float a = (par_a[0] - 0.5) * 0.45;
    float s = sin(a);
    float c = cos(a);

    float z = mix(0.88, 1.14, par_a[1]);

    vec2 q;
    q.x = p.x * c - p.y * s;
    q.y = p.x * s + p.y * c;

    q /= z;

    vec2 src_uv = q + 0.5;

    vec2 self = prev_here.rg * 2.0 - 1.0;
    float e = lum(prev_here.rgb);

    src_uv += self * e * mix(0.0, 0.18, par_a[3]);
    src_uv = fract(src_uv);

    vec3 fb = texture2D(bfr, src_uv).rgb;

    float keep = mix(0.80, 0.999, par_a[2]);
    fb *= keep;

    vec3 init = texture2D(tex[0], uv).rgb;

    vec3 rgb = mix(init, fb, state);

    gl_FragColor = vec4(rgb, color.a);
}