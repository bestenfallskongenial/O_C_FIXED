precision mediump float;

// Shifting Colonnade
//
// Repeating rounded structures sweep past a traveling viewpoint, appearing as curved ribs, striped walls, and grainy surfaces. Their contours fold and dissolve as the view turns, like a colonnade continually rebuilding itself.
//
// par_a.y - changes XY viewing-ray scale, altering framing and distortion.
// par_a.z - changes the rounded-box distance contribution, reshaping surfaces and shading.
// par_a.w - changes XZ viewing scale and sampling distance.

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

float f0 = mix(0.15, 0.95, par_a[1]);   //f0:zoom:
float f1 = mix(0.05, 0.5, par_a[2]);    //f1:spine:
float f2 = mix(0.3, 0.95, par_a[3]);    //f2:zoom2:

float sdBoxXY(vec3 p, vec3 b)
{
    vec2 d = abs(p.xy) - b.xy;
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float udRoundBox(vec3 p, vec3 b, float r)
{
    return length(max(abs(p) - b, 0.0)) - r;
}

float smin(float a, float b, float k)
{
    float res = exp(-k * a) + exp(-k * b);
    return -log(res) / k;
}

mat2 rot(float x)
{
    return mat2(cos(x), sin(x), -sin(x), cos(x));
}

float map(vec3 p)
{
    float k = 0.5 * 2.0;
    vec3 q = (fract((p - vec3(0.25, 0.0, 0.25)) / k) - 0.5) * k;
    vec3 s = vec3(q.x, p.y, q.z);
    float d = udRoundBox(s, vec3(0.1, 1.0, 0.1), 0.05);
    k = 0.5;
    s = vec3(q.x, abs(p.y) - 1.5, q.z);
    float g = udRoundBox(s, vec3(0.17, 0.5, 0.17), 0.2) * f1;
    float sq = sqrt(0.5);
    vec3 u = p;
    d = max(d, -sdBoxXY(u, vec3(0.8, 1.0, 0.8)));
    return smin(d, g, 16.0);
}

vec3 normal(vec3 p)
{
    vec3 o = vec3(0.001, 0.0, 0.0);
    return normalize(vec3(
        map(p + o.xyy) - map(p - o.xyy),
        map(p + o.yxy) - map(p - o.yxy),
        map(p + o.yyx) - map(p - o.yyx)
    ));
}

float trace(vec3 o, vec3 r)
{
    float t = 1.0;
    t += 2.0 * atan(map(o + r * time * t));
    t += cos(map(o + r * t));
    t += map(o + r * t);
    return t;
}

void main()
{
    vec2 uv = gl_FragCoord.xy / tres.xy;
    uv = uv * 2.0 - 1.0;
    uv.x *= tres.x / tres.y;
    float gt = time / 5.0;
    vec3 r = normalize(vec3(uv, 1.7 - dot(uv, uv) * 0.1));
    float sgt = sin(gt * 3.141592 * 2.0);
    r.xy *= rot(sgt * 3.141592 / 8.0) * f0;
    r.xz *= rot(gt * 3.141592 * 2.0) * f2;
    r.xz *= rot(3.141592 * -0.25);
    vec3 o = vec3(0.0, 0.0, gt * 5.0 * sqrt(2.0) * 2.0);
    o.xz *= rot(3.141592 * -0.25);
    float t = trace(o, r);
    vec3 w = o + r * t * f2;
    vec3 sn = normal(w);
    float fd = map(w);
    vec3 col = vec3(0.514, 0.851, 0.933) * 0.5;
    vec3 ldir = normalize(vec3(-1, -0.5, 1.1));
    float front = max(dot(r, -sn), 0.0);
    float ref = max(dot(r, reflect(-ldir, sn)), 0.0);
    float grn = pow(abs(sn.y), 3.0);
    vec3 cl = vec3(grn);
    cl += mix(col * vec3(1.5), vec3(0.25), grn) * pow(ref, 16.0);

    gl_FragColor = vec4(cl, color.a);
}