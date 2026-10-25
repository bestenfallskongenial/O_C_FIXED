precision mediump float;

// Drifting Archipelago
//
// A layered noise landscape scrolls diagonally beneath the viewpoint. Successive height bands suggest shorelines, inland ground, and elevated ridges. Two external images can be blended together and gradually replace the generated landscape.
//
// par_a.x - increases terrain detail and reduces the apparent size of its features.
// par_a.y - blends between the first and second source textures.
// par_a.z - blends from the generated terrain toward the combined source textures.

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

vec3 mod289(vec3 x)
{ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x)
{ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x)
{ return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187, 0.366025403784439,
             -0.577350269189626, 0.024390243902439);
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v -   i + dot(i, C.xx);
    vec2 i1;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod289(i);
    vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0))
    + i.x + vec3(0.0, i1.x, 1.0));
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0 + h*h);
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

float fbm(vec2 x)
{
    float v = 0.0;
    float a = 0.5;
    vec2 shift = vec2(100.0);
    mat2 rot = mat2(cos(0.5), sin(0.5), -sin(0.5), cos(0.50));
    for (int i = 0; i < 5; ++i)
{
        v += a * snoise(x);
        x = rot * x * 2.0 + shift;
        a *= 0.5;
    }
    return v;
}

void main()
{
    vec2 uv = tcoord * 10.0 * par_a[0];
    uv += time * 0.1;
        float n = fbm(uv);
        vec3 water = vec3(0.0, 0.2, 0.5);
    vec3 sand = vec3(0.8, 0.7, 0.4);
    vec3 grass = vec3(0.1, 0.5, 0.1);
    vec3 rock = vec3(0.5, 0.5, 0.5);
    vec3 snow = vec3(1.0);
        vec3 col = water;
    col = mix(col, sand, smoothstep(0.0, 0.1, n));
    col = mix(col, grass, smoothstep(0.2, 0.3, n));
    col = mix(col, rock, smoothstep(0.4, 0.5, n));
    col = mix(col, snow, smoothstep(0.6, 0.7, n));
        vec3 texCol = mix(texture2D(tex[0], tcoord).rgb, texture2D(tex[1], tcoord).rgb, par_a[1]);
    col = mix(col, texCol, par_a[2]);
        gl_FragColor = vec4(col, 1.0);
}