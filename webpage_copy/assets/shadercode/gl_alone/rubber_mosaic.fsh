precision mediump float;

// Rubber Mosaic
//
// A repeating geometric field bends through a radial distortion driven by flowing noise. Its regions swell, compress, and slide across one another, like a tiled surface stretched beneath a moving membrane.
//
// par_a.x - changes the red and green channel mapping.
// par_a.y - changes pattern intensity before color mapping, shifting the resulting contours.
// par_a.z - increases radial scale and distortion, making the pattern more densely divided.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:color_out offset 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:color_out offset 2:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:color_out detail:

vec2 Distort(vec2 p)
{
    float theta = atan(p.y, p.x);
    float radius = length(p) * f2;
    radius = pow(radius, 1.3);
//  radius = radius * radius * radius;  // simplified from pow    
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

vec4 pattern(vec2 p)
{
    vec2 m = mod(p.xy + p.x + p.y, 2.0) - 1.0;
    return vec4(length(m + p * 0.1)) / f1;
}

float hash(const float n)
{
    return fract(sin(n) * 43758.5453);
}

float noise(const vec3 x)
{
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + p.z * 43.0;
    float r1 = mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
                   mix(hash(n + 57.0), hash(n + 57.0 + 1.0), f.x), f.y);
    float r2 = mix(mix(hash(n + 43.0), hash(n + 43.0 + 1.0), f.x),
                   mix(hash(n + 43.0 + 57.0), hash(n + 43.0 + 57.0 + 1.0), f.x), f.y);
    return mix(r1, r2, f.z);
}

void main()
{
    vec2 position = (gl_FragCoord.xy / tres.xy) * 8.0;
    float off = noise(position.xyx + time);
    vec4 c = pattern(Distort(position * off));
    c.xy = Distort(c.xy) * f0;
    vec3 color_out = vec3(c.x * off, pow(sin(c.y) - off, 0.5), pow(cos(c.z), 3.0));

    gl_FragColor = vec4(color_out, color.a);
}