precision mediump float;

// Double Tear Ribbon
//
// A broad waving ribbon is interrupted by two horizontal displacement regions. Their positions, heights, and shared sideways shift change in half-second steps, cutting sudden offsets through the otherwise smooth rolling boundaries.
//
// par_a.x - increases coordinate scale, revealing more of the wave pattern.
// par_a.y - narrows the shading band around the second wave boundary.
// par_a.z - narrows the shading band around the first wave boundary.
// par_a.w - increases the sideways displacement within the two tear regions.

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

// Stable random function
float rand(float n) {
    return fract(sin(n) * 43758.5453123);
}

void main(void) {
    float changeRate = 0.5;
    float timeStep = floor(time / changeRate);
    
// Upper bar (0% to 50% of screen height)
   float stableRand1Upper = rand(timeStep * 0.5);   // -> controls vertical position of the bar
   float stableRand2Upper = rand(timeStep * 0.7);   // -> controls thickness/height of the bar
   float stableRand3Upper = rand(timeStep * 0.4);   // -> controls amount of horizontal shift
   
   float barCenterUpper = tres.y * (0.2 + stableRand1Upper * 0.1);
// 0.2 = minimum position (20% from top)
// 0.3 = range of movement (up to 40%)
   float barWidthUpper = tres.y * (0.05 + stableRand2Upper * 0.35);
// 0.05 = minimum thickness (5% of screen)
// 0.35 = maximum additional thickness (up to 40%)
   float shiftUpper = (stableRand3Upper - 0.5) * par_a.w * 0.5 * tres.x;
// 0.5 = centers the shift range (+/-)
// 0.3 = maximum shift amount (30% of screen width)
   // Lower bar (50% to 100% of screen height)
   float stableRand1Lower = rand(timeStep * 0.5);   // -> controls vertical position of the bar
   float stableRand2Lower = rand(timeStep * 0.7);   // -> controls thickness/height of the bar
   float stableRand3Lower = rand(timeStep * 0.4);   // -> controls amount of horizontal shift
   
   float barCenterLower = tres.y * (0.7 + stableRand1Lower * 0.1); // Position between 60-90% height
// 0.1 = minimum position (10% from top)
// 0.3 = range of movement (up to 40%)
   float barWidthLower = tres.y * (0.05 + stableRand2Lower * 0.35);
// 0.05 = minimum thickness (5% of screen)
// 0.15 = maximum additional thickness (up to 20%)
   float shiftLower = (stableRand3Lower - 0.5) * par_a.w * 0.5 * tres.x;
// 0.5 = centers the shift range (+/-)
// 0.3 = maximum shift amount (30% of screen width)
    vec2 position = gl_FragCoord.xy;
    
    // Apply shifts for both bars
    if (abs(position.y - barCenterUpper) < barWidthUpper) {
        position.x += shiftUpper;
    }
    if (abs(position.y - barCenterLower) < barWidthLower) {
        position.x += shiftLower;
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