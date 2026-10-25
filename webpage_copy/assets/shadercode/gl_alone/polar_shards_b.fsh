precision mediump float;

// Eclipse Mosaic
//
// An unbroken dark field fills the screen.
//
// par_a.x - scales radial coordinates and changes the upper-half animation phase.
// par_a.y - increases the blue channel only.
// par_a.z - scales angular coordinates, changing mosaic placement.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:bump height:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:color_out cycle:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:horz offset:

float rand(vec2 co)
{
    return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

int getClosestPoint(vec2 o)
{
    int closest = 0;
    float dist = 8.0;
    for(int i = 1; i <= 20; i++)
        {
        float fi = float(i);
        vec2 seed = vec2(fi);
        vec2 p = -1.0 + 2.0 * vec2(rand(seed), rand(seed * 125.0));
        if(length(p - o) < dist)
            {
            dist = length(p - o);
            closest = i;
            }
        }
    return closest;
}

void main()
{
    vec2 pos = -1.0 + 2.0 * gl_FragCoord.xy / tres;
    pos.x *= (tres.x / tres.y);
    vec3 color_out = vec3(0.1, 0.1, 0.1);
    float u = length(pos) * f0;
    float v = atan(pos.y, pos.x) * f2;
    float t = 8.0 * time - 1.0 / pos.y;
    if(pos.y > 0.0) t = 8.0 * time + 1.0 / u;
    float fid = float(getClosestPoint(vec2(sin(t) * u, v)));
    color_out.x = rand(vec2(fid, fid * 501.0));
    color_out.y = rand(vec2(fid, fid * 511.0));
    color_out.z = rand(vec2(fid, fid * 521.0)) * f1;
    vec3 tex = color_out * abs(pos.y * 4.0);
    if(pos.y > 0.0) tex *= (u * 4.0);

    gl_FragColor = vec4(tex, color.a);
}