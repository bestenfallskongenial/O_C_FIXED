precision mediump float;

// Stained Ripples
//
// Three families of concentric rings overlap around slowly orbiting centers. Their changing spacing cuts the image into irregular enclosed cells, continually splitting and joining like intersecting ripples held inside a mosaic.
//
// par_a.x - thickens the outlines.
// par_a.y - increases coordinate scale, showing smaller circles and more rings.
// par_a.z - shifts the third family of rings radially, changing their intersections.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:outline thickness:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:zoom:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:circle1 radius:

void main(void)
{
    vec2 position = (2.0 * gl_FragCoord.xy - tres) / tres.xx * f1 + 0.25;
    vec3 color_out;
    float time2 = 0.2 * time;
    float time3 = 0.1 * time;
    float time4 = 0.15 * time;
    float time5 = 0.2 * time;
    float speed1 = 0.2 * time2;
    float range1 = 0.3;
    float ripple1 = 10.0 + 5.0 * sin(time2);
    float speed2 = -0.3 * time2;
    float range2 = 0.5;
    float ripple2 = 15.0 + 10.0 * sin(time2 + 3.0);
    float speed3 = -0.3 * time2;
    float ripple3 = 5.0 + 4.0 * sin(time2 * 2.0 + 2.0);
    float range3 = 0.3;
    vec2 p1 = vec2(0.0, 0.0) + vec2(range1 * cos(speed1), range1 * sin(speed1));
    vec2 p2 = vec2(0.2, 0.0) + vec2(range2 * cos(speed2), range2 * sin(speed2));
    vec2 p3 = vec2(0.0, 0.3) + vec2(range3 * cos(speed3), range3 * sin(speed3));
    float r1 = length(position - p1) + 0.5;
    float r2 = length(position - p2) + 0.5;
    float r3 = length(position - p3) + 0.5 * f2;
    float t1 = fract(r1 * ripple1) / ripple1;
    float t2 = fract(r2 * ripple2) / ripple2;
    float t3 = fract(r3 * ripple3) / ripple3;
    float c = 1000.0 * floor(time * 0.01) + floor(r1 * ripple1) + 10.0 * floor(r2 * ripple2) + 100.0 * floor(r3 * ripple3);
    float rand = sin(c);
    color_out.rgb = fract(rand * vec3(10.0, 1000.0, 100000.0));
    float thr = 0.04 * f0;
    if (t1 < thr || t2 < thr || t3 < thr)
        color_out = vec3(0.0, 0.0, 0.0);

    gl_FragColor = vec4(color_out, color.a);
}