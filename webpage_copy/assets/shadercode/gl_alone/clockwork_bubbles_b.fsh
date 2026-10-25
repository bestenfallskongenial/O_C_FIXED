precision mediump float;

// Counterflow Discs
//
// Two fields of pulsing circles slide in opposite directions. Their overlaps cancel into holes, crescents, and broken rings, continually scattering and rebuilding the arrangement like bubbles passing through their own reflections.
//
// par_a.x - increases horizontal drift speed.
// par_a.y - increases vertical drift speed.
// par_a.z - increases the number of circles, making them smaller.

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
float f1 = mix(0.5, 0.95, par_a[1]);    //f1:y offset:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:scale:

float circle(vec2 coord, float spd)
{
    float reso = 2.0 + (20.0 * f2);
    float cw = tres.x / reso;
//  vec2 p = mod(coord, cw);                    // <-normal circles
    vec2 p = mod(coord - tres * 0.5, cw);       // <-scattered circles
    float d = distance(p, vec2(cw / 2.0));

    float rnd = dot(floor(coord / cw), vec2(1323.443, 1412.312));
    float t = time * 2.0 + fract(sin(rnd)) * 6.2;

    float l = cw * (sin(t * spd) * 0.25 + 0.25);
    return clamp(l - d, 0.0, 1.0);
}

void main()
{
//  vec2 p = gl_FragCoord.xy;                   // <-normal circles
    vec2 p = (tcoord - 0.5) * tres;             // <-scattered circles
    vec2 dp = vec2(7.9438 * f0 * 0.5, 1.3335 * f1) * time;
    float c1 = circle(p - dp, 1.0);
    float c2 = circle(p + dp, 1.4);
    float c = max(0.0, abs(c1 - c2));
    gl_FragColor = vec4(c, c, c, color.a);
}