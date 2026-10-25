precision mediump float;

// Recursive Canvas Stamp
//
// A source image stamps a selected region of the canvas while bfr carries earlier impressions through gentle rotation and scaling. Repeated copies gather into trailing arrangements, letting a single placed image grow into a layered composition.
//
// par_a.x - feedback magnification.
// par_a.y - rotation per feedback step; the midpoint stops rotation.
// par_a.z - persistence of earlier impressions.
// par_a.w - strength of the fresh image stamp.
// par_b.x - horizontal stamp position.
// par_b.y - vertical stamp position.
// par_b.z - stamp size.

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
        Backbuffer transform.
        This is the important part:
        we do NOT transform tex[0].
        We transform the previous framebuffer image.
    */

    vec2 p = uv - 0.5;

    float zoom = mix(0.98, 1.04, par_a.x);
    p /= zoom;

    float ang = (par_a.y - 0.5) * 0.08;
    float s = sin(ang);
    float c = cos(ang);

    vec2 bpuv;
    bpuv.x = p.x * c - p.y * s;
    bpuv.y = p.x * s + p.y * c;

    bpuv += 0.5;

    vec4 old = texture2D(bfr, bpuv);

    float insideOld =
        step(0.0, bpuv.x) *
        step(0.0, bpuv.y) *
        step(bpuv.x, 1.0) *
        step(bpuv.y, 1.0);

    old *= insideOld;

    /*
        New source stamp.
        This is the "blit source".
        Position and size controlled by par_b.
    */

    vec2 pos  = par_b.xy;
    float scl = mix(0.10, 0.60, par_b.z);

    vec2 suv = (uv - pos) / scl + 0.5;

    vec4 src = texture2D(tex[0], suv);

    float insideSrc =
        step(0.0, suv.x) *
        step(0.0, suv.y) *
        step(suv.x, 1.0) *
        step(suv.y, 1.0);

    src *= insideSrc;

    /*
        Canvas compose.
    */

    float decay  = mix(0.90, 0.995, par_a.z);
    float inject = par_a.w;

    vec3 rgb = old.rgb * decay;

    rgb = mix(rgb, src.rgb, src.a * insideSrc * inject);

    gl_FragColor = vec4(rgb, color.a);
}