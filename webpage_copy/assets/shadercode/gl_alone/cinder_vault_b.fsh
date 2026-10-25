precision mediump float;

// Echo Vault
//
// Repeating curved structures pass a moving viewpoint, their surfaces broken into irregular patches of light and shadow. Fragments emerge, stretch, and retreat as the view advances, like brief reflections inside a winding vault.
//
// par_a.x - rotates the image plane.
// par_a.y - turns the viewing direction in ZX.
// par_a.z - lowers the procedural noise value, changing surface brightness and coverage.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:rotation 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:horz spin:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:tex detail:

vec2 rot(vec2 p, float a)
{
    return vec2(
        p.x * cos(a) - p.y * sin(a),
        p.x * sin(a) + p.y * cos(a)
    );
}

float map(vec3 p)
{
    float k = cos(p.x) + cos(p.y);
    k = max(k, -(length(abs(mod(p.yz, 6.0)) - 3.0) - 1.0));
    k = max(k, -(length(abs(mod(p.xz, 6.0)) - 3.0) - 1.5));
    return k;
}

vec4 hash(vec4 n)
{ 
    return fract(sin(n) * 43758.5453123); 
}

float texy(in vec3 x)
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;
    vec4 a = hash(n + vec4(0.0, 57.0, 113.0, 170.0));
    vec4 b = hash(n + vec4(1.0, 58.0, 114.0, 169.0));
    float res = mix(mix(mix(a.x, b.x, f.x), mix(a.y, b.y, f.x), f.y),
                    mix(mix(a.z, b.z, f.x), mix(a.w, b.w, f.x), f.y), f.z);
    return res * 2.0 - 1.0 * f2;
}

void main()
{
    vec3 pos = vec3(0.0, 0.0, time * 5.0);
    vec3 dir = normalize(vec3((-1.0 + 2.0 * (gl_FragCoord.xy / tres.xy)) * vec2(tres.x / tres.y, 1.0), 1.0));
    float t = 0.0;
    dir.xy = rot(dir.xy, f0 * 6.0);
    dir.zx = rot(dir.zx, f1 * 10.1);
    
    for(int i = 0; i < 6; i++) 
        {
        t += map(pos + dir * t) * 0.98;
        }
    vec3 inter = vec3(pos + dir * t);
    vec3 c1 = vec3(1.0, 2.0, 3.0);
    vec3 col = mix(c1, c1.zyx, t * 0.1) * clamp(texy(inter * 2.0) * 20.0, -1.0, 2.0);
    col = sqrt(col * 0.01) * (map(inter + normalize(vec3(1.0, 2.0, 3.0))) * 3.0);
    
    gl_FragColor = vec4(col + t * 0.02, color.a);
}