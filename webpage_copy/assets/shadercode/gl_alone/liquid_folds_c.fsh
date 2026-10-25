precision mediump float;

// Molten Silk
//
// Interwoven ribbons buckle into curling seams, narrow folds, and broad flowing channels. Their continuous distortion pulls the surface into marbled eddies, like wet silk twisting through a current.
//
// par_a.x - increases horizontal warping.
// par_a.y - increases vertical warping.
// par_a.z - increases brightness and white clipping.

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
//f0:mod 1:
//f1:mod 2:
//f2:darkness:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

void main() {
    vec2 p = gl_FragCoord.xy / tres.x - 0.62;
    vec3 col;
    
    for(float j = 0.0; j < 3.0; j++) {
        for(float i = 1.0; i < 4.0; i++) {
            p.x += 0.3 * f0 / (i + j) * sin(i * 15.87654321 * p.y + time + cos((time / (7.0 * i)) * i + j));
            p.y -= 0.4 * f1 / (i + j) * cos(i * 5.0 * p.x + time + sin((time / (5.3456 * i)) * i + j));
        }
        col[int(j)] = abs(p.x / p.y * 2.0 * f2);
    }
    
    gl_FragColor = vec4(col, color.a);
}