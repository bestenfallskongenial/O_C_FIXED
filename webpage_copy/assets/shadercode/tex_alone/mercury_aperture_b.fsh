precision mediump float;

// Inward Bloom
//
// An external image bends into repeating curved fragments that gather toward a glowing center. Rapid scrolling carries its details through the folds, while softly blended samples make the surface shimmer inside a narrow rectangular rim.
//
// par_a.x - increases horizontal texture repetition and distortion toward the center.
// par_a.y - increases vertical texture repetition and distortion toward the center.

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

const float pi = 3.14159265;

void main(void)
{
    vec2 mouse;
    mouse.x = par_a[0];
    mouse.y = par_a[1];

    vec2 p = (gl_FragCoord.xy / tres.xy) * 2.0 - 1.0;
    float a = atan(p.y, p.x);
    float r = sqrt(dot(p, p));
    vec2 uv = vec2(0.0, 0.0);
    uv.x = mod(mouse.x * cos(a) / r + time * 500.0, 1.0);
    uv.y = mod(mouse.y * sin(a) / r + time * 600.0, 1.0);
    
    float amount = sin(time * 5000.0) * 0.01;
    vec4 color0 = texture2D(tex[0], uv);
    vec4 color1 = texture2D(tex[0], uv + vec2(0.0, -amount));
    vec4 color2 = texture2D(tex[0], uv + vec2(0.0, amount));
    vec4 color3 = texture2D(tex[0], uv + vec2(amount, 0.0));
    vec4 color4 = texture2D(tex[0], uv + vec2(-amount, 0.0));
    vec4 fragColor = ((color0 + color1 + color2 + color3 + color4) / 8.0) + pow(1.0 - r, 3.0);
    
    float border = 0.95;
    if (p.x < -border || p.x > border || p.y < -border || p.y > border)
{
        fragColor = vec4(p.x + p.y);
    }

    gl_FragColor = fragColor * color.a;
}