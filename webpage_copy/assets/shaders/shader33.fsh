// www.glslsandbox.com/e#108578.0                         
//----------------------------------------// shader name length of 40 characters

precision mediump float;

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:accent lines:
float f1 = mix(0.2, 0.8, par_a[1]);     //f1:spatial pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:squeeze:

vec2 mouse = par_a.xy;

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec3 c;
    float l, z = time;
    for (int i = 0; i < 3; i++) {
        vec2 uv, p = fragCoord.xy / tres.xy;
        uv = p;
        p -= 0.5;
        p.x *= tres.x / tres.y;
        z += 0.07;



        l = length(p);
        uv += p / l * (sin(z) + 1.0) * abs(sin(l * 9.0 - z - z));

        // Use mouse input to modify the shader behavior
        uv += mouse.xy; // keep the divider here / tres.xy; // * 0.1; // Take the first two components of mouse

        c[i] = 0.01 / length(mod(uv, 1.0) - 0.5);
    }
    fragColor = vec4(c / l, time);
    fragColor.a = color.a; 
}

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}
