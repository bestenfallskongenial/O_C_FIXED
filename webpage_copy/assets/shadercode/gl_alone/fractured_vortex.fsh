precision mediump float;

// Fractured Vortex
//
// Fan-like wedges and nested contours sweep toward shifting convergence points. Their surfaces buckle into broken grids and sharp seams, creating a restless vortex framed by horizontal bars.
//
// par_a.x - changes the speed of the rotating distortion.
// par_a.y - increases the top and bottom bar height while changing the vertical shaping of the main effect.

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

vec2 mouse = par_a.xy;

void main(void) {
    vec2 p = (gl_FragCoord.xy * 2.0 - tres) / min(tres.x, tres.y);
    vec3 color_out = vec3(0.0, 0.3, 0.5);
    float d = 0.8-smoothstep(0.0,1.8,length(p));
    float f = 0.0;
    float PI = 3.141592;

    // Calculate black bar size (10% to 40% range, with 20% as middle)
    float barSize = mix(0.10, 0.60, mouse.y);

    // Check if we're in the black bar region
    if (abs(p.y) > (1.0 - barSize)) {
        gl_FragColor = vec4(0.0, 0.0, 0.0, color.a);  // Black bars
        return;
    }

    // Original effect for the main area
    for(float i = 0.0; i < 4.0; i++) {
        float s = sin(mouse.x*.715*time + i * PI / 8.0) * 10.01;
        float c = cos(mouse.x*.715*time + i * PI / 8.0) * 8.5;
        f += mod(sin(time)*p.y+cos(time)*p.x, 1.-p.y) / ( floor(-d-fract(12.*d)+2.5) + p.x * c + fract(.35*time-2.*d) + p.y * s);
    }

    gl_FragColor = vec4(max(cos(acos(mouse.y*1.7*p.y) *f + color_out),1.-vec3(fract(2.*time), 0, 0.) - (mod(f,fract(time+12.*abs(p.x)-2.*p.y)) + acos(1.05*abs(p.y)))), color.a);
}