precision mediump float;

// Checkered Horizon
//
// A scrolling checkerboard recedes toward a bending horizon. Waves lift and lower its distant edge, turning the ground into rolling hills beneath a broad atmospheric glow, like a landscape viewed from a drifting platform.
//
// par_a.x - changes the complexity of the horizon’s smaller undulations.
// par_a.y - increases the checkerboard ground’s intensity.
// par_a.z - changes perspective depth and vertical checker spacing, while strengthening the horizon glow.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:hills:
float f1 = mix(0.0, 0.95, par_a[1]);    //f1:ground:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:sunrise:

void main()
{
    vec2 pos = (gl_FragCoord.xy / tres.xy) - vec2(0.5, 0.5);     
    pos.y += sin(time + pos.x * 3.1) * 0.1;
    float vv = sin(time * 0.65 + pos.y + pos.x * 7.664) * 5.0 * f0;
    pos.y -= sin(vv - sin(time - pos.y * 8.0)) * 0.04;
    vec3 p = vec3(pos.x, f2, pos.y + 0.0);  
    vec2 s = vec2(p.x / p.z, p.y / p.z) * 0.1;
    s.x = s.x - time * 0.07;
    float c = sign((mod(s.x - time * 0.1, 0.05) - 0.025) * (mod(s.y - time * 0.15, 0.05) - 0.025));
    c *= max(0.3, p.z * p.z * 29.0) * f1;
    
    vec3 color_out;
    if (pos.y <= 0.0) 
        {
        color_out = vec3(0.0, c * 0.5 + sin(time * 0.6) * 0.1, c);
        }
    else 
        {
        color_out = vec3(abs(s.y * 5.0) * 0.5, abs(s.y * 1.4) * 0.5, 0.3);
        }
    gl_FragColor = vec4(color_out, color.a);
}