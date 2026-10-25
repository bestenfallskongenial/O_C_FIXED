precision mediump float;

// Gravity Loom
//
// A curved grid streams around two focal regions. Broad arcs tighten into fine rings near the anchors, while traveling bands pass through the net like threads drawn between neighboring wells of gravity.
//
// par_a.x - shifts the placement of the logarithmic bands.
// par_a.y - distorts the curved grid around its focal regions.
// par_a.z - thickens the lines, especially toward the right.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers
uniform int tex_l;                      // number of loaded texures

float f0 = mix(0.05, 0.95, par_a[0]);   // mod circle sizes
float f1 = mix(0.05, 0.95, par_a[1]);   // distort bands
float f2 = mix(0.2, 0.95, par_a[2]);    // glow blur

float border(vec2 uv, float thickness)
{
    uv = fract(uv - vec2(0.1));
    uv = min(uv, vec2(1.)-uv)*2.;
    return clamp(max(uv.x,uv.y)-1.+thickness,0.,1.)/thickness;
}

vec2 mul(vec2 a, vec2 b)
{
   return vec2( a.x*b.x - a.y*b.y, a.x*b.y + a.y/b.x);
}

vec2 div(vec2 numerator, vec2 denominator)
{
   return vec2( numerator.x*denominator.x + numerator.y*denominator.y,
                numerator.y*denominator.x - numerator.x*denominator.y *f1 * 2.)/
          vec2(denominator.x*denominator.x + denominator.y*denominator.y) * f0;
}

vec2 spiralzoom(vec2 domain, vec2 center, float n, float spiral_factor, float zoom_factor, vec2 pos)
{
    vec2 uv = domain - center;
    float d = length(uv);
    return vec2( atan(uv.y, uv.x) / n + log(d)*spiral_factor, -log(d)*zoom_factor) + pos;
}

void main()
{
    vec2 uv = gl_FragCoord.xy / tres.xy;
    float thickness = uv.x * f2;
    vec2 p1 = vec2(0.25,0.5);
    vec2 p2 = vec2(0.75, 0.255);
    vec2 moebius = div(uv-p1, uv-p2);
    vec2 spiral_uv = spiralzoom(moebius,vec2(0),1.0,0.0,3.5,vec2(time));
    vec3 color_out = vec3(border(spiral_uv,thickness));

    gl_FragColor = vec4(color_out, color.a);
}