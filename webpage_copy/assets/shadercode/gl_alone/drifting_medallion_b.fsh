precision mediump float;

// Lagoon Medallion
//
// Overlapping circles form a central disc surrounded by offset rings. Their centers drift gently out of alignment, making the borders thicken on one side and narrow on the other; larger settings introduce additional layers.
//
// par_a.x - increases horizontal movement of the circle centers.
// par_a.y - increases vertical movement of the circle centers.
// par_a.w - enlarges the circles and increases the number of layers in steps.

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
 
float sdCircle(vec2 p, float r) {
    return length(p) - r;
}

const vec3 poison_apple = vec3(0.635, 0.0, 0.38);
const vec3 teal_blue = vec3(0.0, 0.329, 0.533);
const vec3 misty_green = vec3(0.811, 0.894, 0.819);
const vec3 darkblue_green = vec3(0.0, 0.435, 0.482);
const vec3 medium_gray = vec3(0.623, 0.627, 0.627);  
void main(void)
{
    vec2 p = (gl_FragCoord.xy * 2.0 - tres) / min(tres.x, tres.y);
    vec3 col = vec3(0.0);
    vec2 vec;
    float d;

    float sizeFactor = mix(0.5, 4.5, par_a[3]);
        const float MAX_ITERATIONS = 4.0;
        for(float i = 0.0; i < MAX_ITERATIONS; i++)
    {
        if(i >= mix(2.0, MAX_ITERATIONS, par_a[3])) break;
       vec = vec2(cos(i *  time) * (i * 0.04 * par_a[0]),
                   sin(i *  time) * (i * 0.04 * par_a[1]));
        d = sdCircle(p + vec, i * 0.07 * sizeFactor);
       if(d > 0.0)
{
            if(mod(i, 5.0) < 1.0) col = teal_blue;
            else if(mod(i, 5.0) < 2.0) col = poison_apple;
            else if(mod(i, 5.0) < 3.0) col = misty_green;
            else if(mod(i, 5.0) < 4.0) col = darkblue_green;
            else col = medium_gray;
        }
        col = mix(col, vec3(0.0), smoothstep(0.01, 0.0, abs(d)));
    }
        gl_FragColor = vec4(col, color.a );
}