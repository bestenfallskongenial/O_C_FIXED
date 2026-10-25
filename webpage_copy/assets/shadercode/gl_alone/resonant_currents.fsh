precision mediump float;

// Resonant Currents
//
// Horizontal, vertical, and circular waves combine into broad flowing patches. Their intersections swell and retreat as the phases advance, while low-frequency audio strengthens the field, making its contours respond like currents stirred by a changing pulse.
//
// par_a.x - increases horizontal wave frequency and red-channel strength.
// par_a.y - increases vertical wave frequency and green-channel strength.
// par_a.z - increases radial wave frequency and blue-channel strength.

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


float f0 = mix(0.05, 0.95, par_a[0]);  
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

void main() {
    vec2 uv = tcoord - 0.5;
    
    // Create plasma effect using sine waves
    float plasma = sin(uv.x * f0 * 10.0 + time) 
                + sin(uv.y * f1 * 10.0 + time)
                + sin(sqrt(uv.x * uv.x + uv.y * uv.y) * f2 * 10.0 + time);
                
    // Add audio reactivity
    plasma *= 1.0 + audio.x * 2.0;
    
    // Create color gradient
    vec3 col = vec3(f0 * sin(plasma), 
                    f1 * cos(plasma),
                    f2 * sin(plasma * 0.5));
                    
    // Add seed-based variation
    col = mix(col, col.gbr, seed.x);
    
    gl_FragColor = vec4(col, color.a);
}