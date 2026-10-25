precision mediump float;

// Ivory Veil
//
// A vertically mirrored noise field evolves over a softly shaded paper-like surface. Its symmetry draws the composition toward the center, like a folded ink impression, while a vignette darkens the outer corners.
//
// par_a.x - increases the spatial frequency of the mirrored ink field.
// par_a.y - reduces noise amplitude.
// par_a.z - reduces shading contrast within the ink field.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:complexity:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:density:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:shading:

const vec3 inkColor = vec3(0.01, 0.01, 0.1);
const vec3 paperColor = vec3(1.0, 0.98, 0.94);
float speed = 1.5; 
float shadeContrast = 0.2 / f2;

const float F3 = 0.3333333;
const float G3 = 0.1666667;

vec3 random3(vec3 c)
{
    float j = 4096.0*sin(dot(c,vec3(17.0, 59.4, 15.0)));
    vec3 r;
    r.z = fract(512.0*j);
    j *= .125;
    r.x = fract(512.0*j);
    j *= .125;
    r.y = fract(512.0*j);
    return r-0.5;
}

float simplex3d(vec3 p)
{
    vec3 s = floor(p + dot(p, vec3(F3)));
    vec3 x = p - s + dot(s, vec3(G3));
    
    vec3 e = step(vec3(0.0), x - x.yzx);
    vec3 i1 = e*(1.0 - e.zxy);
    vec3 i2 = 1.0 - e.zxy*(1.0 - e);
    
    vec3 x1 = x - i1 + G3;
    vec3 x2 = x - i2 + 2.0*G3;
    vec3 x3 = x - 1.0 + 3.0*G3;
    
    vec4 w, d;
    
    w.x = dot(x, x);
    w.y = dot(x1, x1);
    w.z = dot(x2, x2);
    w.w = dot(x3, x3);
    
    w = max(0.6 - w, 0.0);
    
    d.x = dot(random3(s), x);
    d.y = dot(random3(s + i1), x1);
    d.z = dot(random3(s + i2), x2);
    d.w = dot(random3(s + 1.0), x3);
    
    w *= w;
    w *= w;
    d *= w;
    
    return dot(d, vec4(52.0));
}

float fbm(vec3 p)
{
    float f = 0.0;
    float frequency = 1.0 + f0;
    float amplitude = 0.5 / f1 + 0.2;
    for (int i = 0; i < 2; i++) 
        {
        f += simplex3d(p * frequency) * amplitude;
        amplitude *= 0.5;
        frequency *= 2.0 + float(i) / 100.0;
        }
    return min(f, 1.0);
}

void main(void)
{
    vec2 uv = 1.0 - gl_FragCoord.xy / tres.xy;
    vec2 coord = 1.0 - uv * 2.0;
    uv.x = 1.0 - abs(1.0 - uv.x * 2.0);
    vec3 p = vec3(uv, time * speed);
    
    float blot = fbm(p * 3.0 + 8.0);
    float shade = fbm(p * 2.0 + 16.0);
    
    blot = (blot + (sqrt(uv.x) - abs(0.5 - uv.y)));
    blot = smoothstep(0.65, 0.71, blot) * max(1.0 - shade * shadeContrast, 0.0);
    
    vec3 color_out = mix(paperColor, inkColor, blot);
    color_out *= 1.0 - pow(max(length(coord) - 0.5, 0.0), 5.0);
    
    gl_FragColor = vec4(color_out, color.a);
}