precision mediump float;

// Turning Squarewell
//
// Nested square contours rotate toward a small central opening. Repeating patches stretch across their surfaces as the field advances, creating the impression of looking into a square well whose walls continually turn and fold inward.
//
// par_a.x - shifts the convergence point horizontally within the rotating field.
// par_a.y - shifts the convergence point vertically within the rotating field.
// par_a.z - increases the frequency of the pattern inside the square contours.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:x offset:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:y offset:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:zoom:

#define R(a) mat2(cos(a), sin(a), -sin(a), cos(a))

float chebyshevDistance(vec2 a, vec2 b)
{    
   a *= 2.5 * R(0.78332691);
   vec2 p = abs(b - a);
   return max(p.x, p.y);
}

float createTexture(in vec2 uv, float T)
{
    return cos(uv.x * 9.0) * sin(uv.y * 9.0);
}

void main(void)
{
    float T = time;
    vec2 uv = (2.0 * gl_FragCoord.xy - tres) / tres.y;
    float tmp = uv.y; // in order to avoid rotation of the canvas
    uv *= R(T * 0.5);
    float d = chebyshevDistance(uv, vec2(f0, f1) - 0.5);
    float t = 1.0 / d * f2;
    vec2 st = uv * t + vec2(0.0, t + T);
    vec3 color_out = vec3(createTexture(st, T));
    vec4 finalColor = mix(acos(tmp * 1.3337) - vec4(3.0, 2.0, 1.0, 0.0), vec4(0.5 / color_out, 1.0), 8.0 / (pow(d, 3.0)));

    gl_FragColor = vec4(finalColor.rgb, color.a);
}