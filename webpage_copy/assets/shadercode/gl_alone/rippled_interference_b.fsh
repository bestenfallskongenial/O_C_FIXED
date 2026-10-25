precision mediump float;

// Chromatic Haze
//
// Expanding circular waves cross a grain-disturbed interference field. Their combined contours drift and pulse, creating broad flowing regions interrupted by fine flickering texture, like ripples passing through unsettled mist.
//
// par_a.x - blends toward inverted colors and reduces brightness; halfway produces uniform gray.
// par_a.y - reduces saturation.
// par_a.z - increases coordinate scale, making the waves more closely spaced.
// par_a.w - rotates the pattern through a full turn.

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

float f0 = mix(0.1, 0.95, par_a[0]);    // f0: :
float f1 = mix(0.05, 0.95, par_a[1]);   // f1: :
float f2 = mix(0.05, 0.95, par_a[2]);   // f2: :

// Noise function
float noise(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

void main()
{
    // Uniform mapping
    float adc5Min = 0.0;
    float adc5Max = 0.5;
    float adc6Min = 0.0;
    float adc6Max = 360.0;
     float zoom = adc5Min + par_a[2] * (adc5Max - adc5Min);
    float rotation = adc6Min + par_a[3] * (adc6Max - adc6Min);
    // UV transformation (zoom)
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
    uv *= zoom;
    // UV transformation (rotation)
    float angle = radians(rotation);
    uv = vec2(uv.x * cos(angle) - uv.y * sin(angle), uv.x * sin(angle) + uv.y * cos(angle));
    // Generate evolving patterns
    float n = noise(uv * 5.0 + time);
    float pattern = sin(uv.x * 10.0 + n) * cos(uv.y * 10.0 + n);
    // Create circular waves
    float dist = length(uv);
    float wave = sin(dist * 20.0 - time * 5.0) * 0.5 + 0.5;
    // Combine patterns
    float combined = mix(pattern, wave, 0.5);
    // Generate color_out
    vec3 color_out = vec3(
        0.5 + 0.5 * sin(combined * 5.0 + time),
        0.5 + 0.5 * sin(combined * 7.0 + time * 1.1),
        0.5 + 0.5 * sin(combined * 9.0 + time * 1.2)
    );
    // Apply effects based on uniforms
    color_out = mix(color_out, 1.0 - color_out, par_a[0]);  // Color inversion
    color_out *= 1.0 - (par_a[0] * 0.5);  // Darkness control
    color_out = mix(color_out, vec3(dot(color_out, vec3(0.299, 0.587, 0.114))), par_a[1]);  // Saturation control

    gl_FragColor = vec4(color_out, color.a);
}