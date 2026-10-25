precision mediump float;

// Wandering Solids
//
// A sphere, box, and ring travel along different axes, passing through one another in a compact spatial dance. Two source images can be blended together and drawn over the moving forms until the picture fills the scene.
//
// par_a.x - blends between the first and second source images.
// par_a.y - blends from the generated forms to the combined source images.

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
 
const float PI = 3.14159265359;
const int MAX_STEPS = 4;
const float MAX_DIST = 10.0;
const float SURF_DIST = 0.01;

float sdSphere(vec3 p, float r)
{
    return length(p) - r;
}

float sdBox(vec3 p, vec3 b)
{
    vec3 q = abs(p) - b;
    return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float sdTorus(vec3 p, vec2 t)
{
    vec2 q = vec2(length(p.xz) - t.x, p.y);
    return length(q) - t.y;
}

float map(vec3 p)
{
    float d1 = sdSphere(p - vec3(sin(time) * 2.0, 0.0, 0.0), 1.0);
    float d2 = sdBox(p - vec3(0.0, sin(time) * 2.0, 0.0), vec3(0.5));
    float d3 = sdTorus(p - vec3(0.0, 0.0, sin(time) * 2.0), vec2(1.5, 0.3));
    return min(min(d1, d2), d3);
}

float rayMarch(vec3 ro, vec3 rd)
{
    float dO = 0.0;
    for(int i = 0; i < MAX_STEPS; i++)
        {
        vec3 p = ro + rd * dO;
        float dS = map(p);
        dO += dS;
        if(dO > MAX_DIST || dS < SURF_DIST) break;
        }
    return dO;
}

vec3 getNormal(vec3 p)
{
    float d = map(p);
    vec2 e = vec2(0.01, 0);
    vec3 n = d - vec3(
        map(p - e.xyy),
        map(p - e.yxy),
        map(p - e.yyx)
    );
    return normalize(n);
}

void main()
{
    vec2 uv = (tcoord - 0.5) * 2.0;
    uv.x *= 16.0 / 9.0;
        vec3 ro = vec3(0, 0, -3);
    vec3 rd = normalize(vec3(uv.x, uv.y, 1));
        float d = rayMarch(ro, rd);
    vec3 p = ro + rd * d;
    vec3 n = getNormal(p);
        vec3 col = n * 0.5 + 0.5;
    col *= 1.0 - d * 0.05;
        vec3 texCol = mix(texture2D(tex[0], tcoord).rgb, texture2D(tex[1], tcoord).rgb, par_a[0]);
    col = mix(col, texCol, par_a[1]);

    gl_FragColor = vec4(col, color.a);
}