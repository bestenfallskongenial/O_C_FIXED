precision mediump float;

// Curved Gauze
//
// Broad curved sheets are crossed by dense meshes of fine marks and broken lines. Small phase changes make the seams shimmer and rearrange, like several layers of gauze sliding across an uneven reflective surface.
//
// par_a.x - increases horizontal pattern frequency.
// par_a.y - increases vertical pattern frequency.
// par_a.z - increases red and green intensity relative to blue.

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

float f0 = mix(0.0005, 0.019, par_a[0]);
float f1 = mix(0.0005, 0.019, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);
float f3 = mix(0.5, 2.0, par_a[3]);
float f4 = radians(mix(0.0, 360.0, par_b[0]));

void main( void )
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
//  uv *= f3;   // UV transformation (zoom)
//  uv = vec2(uv.x * cos(f4) - uv.y * sin(f4), uv.x * sin(f4) + uv.y * cos(f4));    // UV transformation (rotation)
    vec2 position = (uv + 1.0) * tres * vec2(f0, f1);
    float depth = gl_FragCoord.z;
    float color_out = 1.0;
    float my_mod = 5.;
//  color_out += mod(time, 0.1);
//  color_out += dot(position.x, position.y)*depth;
    color_out *= abs(mod(position.x*my_mod*sin(position.x)*position.y+cos(time), position.x*my_mod*cos(position.x)*position.x+sin(time)));
    color_out *= abs(mod(position.y*my_mod*cos(position.y)*position.x+sin(time), position.y*my_mod*sin(position.y)*position.y+cos(time)));
    vec3 finalColor = vec3(color_out* f2, color_out*f2, color_out);

    gl_FragColor = vec4(finalColor, color.a );
}