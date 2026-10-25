precision mediump float;

// Offset Ribbon
//
// A traveling ribbon bends between two smooth wave boundaries. A seed-positioned horizontal strip shifts part of the image sideways, creating a persistent offset through which the moving curves continually pass.
//
// par_a.x - increases coordinate scale, revealing more of the wave pattern.
// par_a.y - narrows the shading band around the second wave boundary.
// par_a.z - narrows the shading band around the first wave boundary.
// par_a.w - increases the seed-controlled sideways displacement.

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

//f0:wave offset:
//f1:green height:
//f2:blue height:
float f0 = mix(0.15, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

vec2 mouse = par_a.xy;

void main(void) {
    // Use seed to create random bar properties
    float barCenter = tres.y * (0.2 + seed.x * 0.6); // Random position between 20% and 80% of screen height
    float barWidth = tres.y * (0.05 + seed.y * 0.15); // Random width between 5% and 20% of screen height
    float shift = (seed.z - 0.5) * par_a.w * 0.3 * tres.x; // Random shift based on par_a.w

    vec2 position = gl_FragCoord.xy;
    
    // If we're in the bar, apply the shift
    if (abs(position.y - barCenter) < barWidth) {
        position.x += shift;
    }
    
    position = position / tres.xy * f0;

    float numberone = cos((position.x + (time*0.1)) * 8.0) * 0.1 + 0.55 + sin(time) * 0.1;
    float numbertwo = cos((position.x + (time*0.1)) * 6.28) * 0.1 + 0.3 + cos(time) * 0.2;
    float dist_one = abs(position.y - numberone) * f2;
    float dist_two = abs(position.y - numbertwo) * f1;

    float red = 0.0;
    float green = 0.0;
    float blue = 0.0;

    if (position.y < numberone && position.y > numbertwo)
        red = 1.0;

    if (dist_one < 0.1)
        blue = dist_one / 0.1;
    if (dist_two < 0.1)
        green = dist_two / 0.1;

    gl_FragColor = vec4(red * 0.5, green * 0.5, blue * 0.5, color.a);
}