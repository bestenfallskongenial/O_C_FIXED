precision mediump float;

// Interlocking Whirl
//
// Two moving ring patterns overlap through a radial inversion. Their shared regions cancel into alternating curved blades and openings, making the surface twist like interlocking ribbons being drawn toward a central whirl.
//
// par_a.y - changes pattern scale and placement while increasing the first ring center’s movement amplitude.
// par_a.z - increases the second ring pattern’s frequency.

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

float f0 = mix(0.25, 0.95, par_a[1]);   //f0:pos:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:offset 1:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:radius 2:

void main(void)
{
    vec2 position = ( gl_FragCoord.xy / tres.xy );
    position -= 0.5;
    position *= 0.15 * pow(length(position), -1.6);
    position += 0.5;
    vec2 p = -1.0 + 2.0 * position * f0;
    
    vec2 offset1 = vec2(0.5*cos(6.0*time), 0.5*sin(3.0*time)) * f1;
    vec2 offset2 = vec2(0.6*sin(3.0*time ), 0.4*cos(2.0*time));
    float radius1 = sqrt(dot(p-offset1, p-offset1));
    float radius2 = sqrt(dot(p-offset2, p-offset2)) * f2;
        
    bool toggle1 = mod(radius1, 0.2) > 0.1;
    bool toggle2 = mod(radius2, 0.2) > 0.1;
    //xor via if statements
    float col = 0.0;
    if (toggle1) col = 1.0;    
    if (toggle2) col = 1.0;
    if ((toggle1) && (toggle2)) col = 0.0;

    gl_FragColor = vec4(col, col, col, color.a );
}