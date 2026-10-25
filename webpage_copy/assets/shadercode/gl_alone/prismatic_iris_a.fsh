precision mediump float;

// Prismatic Iris
//
// Concentric bands gather around an eye-like center while a warped lattice bends across them. Sharp contours flare, fragment, and disappear as the pattern pulses, like an iris opening behind fractured glass.
//
// par_a.x - changes radial mapping and apparent scale.
// par_a.y - changes the amplitude of the oscillating distortion.
// par_a.z - changes a subtractive shaping term, altering bands and dark regions.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of texture samplers       
uniform int tex_l;                      // number of loaded texures 
//f0:zoom:
//f1:side shade:
//f2:circle size:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

#define pi 3.14159265359
#define pi2 6.28318530718

void main(void) {
    vec2 _ = (gl_FragCoord.xy * 2.0 - tres.xy) / tres.y;
    float a = (0.5 + atan(length(_), f0) * pi2);
    float r = clamp(a - acos(_.y * 1.18) / pi2, 0.0, 1.0);
    
    vec2 uv = vec2(1.0 / r + 0.2 * time, a);
    float f = sin(uv.x) * cos(time - uv.y) / f1;
    highp float c = abs(0.01 / sin(f + _.x * 12.0) - f * cos(_.y)) - uv.y * abs(0.0035 / sin(f - _.y * 12.0) - 0.081 / cos(f * _.x)) / f2;
    
    vec3 col = (c * 0.25) + (0.5 * cos(0.25 + (pi2 * c) + vec3(-pi/2.0, pi/2.6, pi)));
    
    vec3 finalColor = c + pow(col + 0.2, 2.2 / vec3(uv.x, uv.y, 1.0 - (uv.x + uv.y)));
    gl_FragColor = vec4(finalColor, color.a);
}