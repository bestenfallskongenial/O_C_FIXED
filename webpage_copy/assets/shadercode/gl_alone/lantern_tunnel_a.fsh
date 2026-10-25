precision mediump float;

// Lantern Tunnel
//
// Rows of luminous rectangular cells wrap around a cylindrical passage. The viewpoint drifts and turns while the tiled walls slide toward a dark opening, resembling a tunnel lined with small lanterns.
//
// par_a.x - increases forward viewing strength, narrowing the field of view.
// par_a.y - widens the horizontal field of view.
// par_a.z - reduces the tunnel radius, bringing its walls closer.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:mod 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:mod 2:
float f2 = mix(0.2, 0.95, par_a[2]);    //f2:mod 3:

const float PI = 3.14159;

vec3 hsv(float h, float s, float v)
{
    float c = s * v;
    float _ = mod(h * 6.0, 6.0);
    vec3 C = vec3(c, c*(1.0 - abs(mod(_, 2.0) - 1.0)), 0.0);
    if (_ < 1.0)
{
        C = vec3(C.x, C.y, C.z);
    }
else if (_ < 2.0)
{
        C = vec3(C.y, C.x, C.z);
    }
else if (_ < 3.0)
{
        C = vec3(C.z, C.x, C.y);
    }
else if (_ < 4.0)
{
        C = vec3(C.z, C.y, C.x);
    }
else if (_ < 5.0)
{
        C = vec3(C.y, C.z, C.x);
    }
else
{
        C = vec3(C.x, C.z, C.y);
    }
    return C + (v - c);
}

float map(vec3 p)
{
    return 0.8 - length(p.xz) * f2;
}

float noise(vec2 co)
{
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void)
{
    vec2 p = (2.0 * gl_FragCoord.xy - tres) / tres.y;
    vec3 cp = vec3(cos(time * 0.2), 0.0, sin(time * 0.45)) * 0.5;
    vec3 cl = vec3(-sin(time), 10.0, cos(time));
    vec3 cf = normalize(cl - cp) * f0;
    vec3 cs = normalize(cross(cf, vec3(sin(time * 0.1), 0.0, cos(time * 0.1)))) * f1;
    vec3 cu = normalize(cross(cs, cf));
    float focus = 0.5;
    vec3 rd = normalize(cs * p.x + cu * p.y + cf * focus);
    vec3 rp = cp;

    for (int i = 0; i < 4; ++i) 
        {
        float d = map(rp);
        if (d < 0.001)
            break;
        rp += (rd * d) * float(i*i + 1*i*i + 1);
        }
    float a = (atan(rp.z, rp.x)) * 16.0 / PI;
    float ai = floor(a); // cell color_out
    float af = fract(a); // cell brightness
    float d = (rp.y + 0.5 * time) * 10.0;
    float di = floor(d);
    float df = fract(d);
    float v = 32.0 * af * (1.0 - af) * df * (1.0 - df) * exp(-rp.y * 0.8);
    vec3 color_out = hsv(noise(vec2(ai, di) * 0.01), 1.0, v);

    gl_FragColor = vec4(color_out, color.a);
}