precision mediump float;

// Hinged Mosaic
//
// Rows of curved facets turn and pulse between broad dark seams. Alternating rows twist differently, breaking the surface into small hinged tiles whose highlights appear and disappear as the pattern folds.
//
// par_a.x - increases pattern density, showing more, smaller elements.
// par_a.y - sets the overall rotation.
// par_a.z - changes highlight formation and visible coverage.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:zoom:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:rotations:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:highlights:

const float PI = 3.1415926535;
const float TWOPI = PI * 2.0;

vec2 rotate(vec2 point, float rads)
{
    float cs = cos(rads);
    float sn = sin(rads);
    return point * mat2(cs, -sn, sn, cs);
}

float curve(float x)
{
    return smoothstep(1.0, 0.0, cos(x * PI)) - cos(x * PI);
}

float thing(vec2 pos)
{
    float offset = 0.0;
    float row = floor((pos.y) / 2.0);
    if (mod(row, 2.0) < 1.0) 
        {
        offset = 1.0;
        pos = rotate(pos, radians(90.0 * sin(time * 0.5)));
        }
    float a = curve(pos.x) - offset + abs(sin(time));
    float b = curve(pos.y - offset * 1.0) - offset * 0.25;
    float c = max(distance(b, a/b), tan(a) - tan(b)) / f2;
    float d = pow(a, c) * abs(sin(-time)) + sqrt(c * a) * abs(sin(time));
    float e = inversesqrt(d);
    return (a * b + d) * (sqrt(a/b)) + c / e;
}

void main()
{
    vec2 position = (gl_FragCoord.xy / tres);
    vec2 world = position * 2.0 - 1.0;
    world *= 5.0 * f0;
    world.x *= tres.x / tres.y;
    world = rotate(world, radians(360.0 * f1));
    float dist = 1.0 / thing(world);
    
    gl_FragColor = vec4(dist, dist, dist, color.a);
}