precision mediump float;

// Breathing Whirlpool
//
// Curved bands spiral around a movable center, gathering into tightly nested loops before opening into broad sweeping lobes. Their radial scale pulses, making the whirlpool appear to inhale and release its surrounding ribbons.
//
// par_a.x - moves the center horizontally.
// par_a.y - moves the center vertically.
// par_a.z - reduces overall brightness as it increases.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:horz pos:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:color_out rotate:

void main(void)
{
    vec2 position = ((-1.0 + 2.0 * gl_FragCoord.xy / tres.xy) - (-1.0 + 2.0 * vec2(f0, f1)));
    
    float a = atan(position.y, position.x);
    float r = sqrt(dot(position, position));
    float s = r * (1.0 + 0.8 * cos(time * 1.0));
    float w = 0.9 + pow(max(1.5 - r, 0.0), 4.0);
    w *= 0.6 + 0.4 * cos(time + 3.0 * a);
    
    position.x = 0.02 * position.y + 0.03 * cos(-time + a * 1.0) / s;
    position.y = 0.05 * time + 0.02 * position.x + 0.03 * sin(-time + a * 2.0) / s;
    position.x = position.x * 1.0001;    
    
    float t = 8.0;
    vec3 color_out = vec3(sin(position.x * cos(t / 2.0) * sin(time * 0.23) * 200.0));
    color_out.r *= sin(position.y * cos(t / 8.0) * 400.0) + cos(time * 2.0) - 2.0;
    color_out.g *= cos(position.x * sin(t / 8.0) * 400.0) + sin(time * 3.0) + 1.5;
    color_out.b -= sin(t / 2.5);
    color_out /= 2.3;
    color_out *= w / f2;  
    
    gl_FragColor = vec4(color_out, color.a);
}