// Transposed Shader tex??                  
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

void main(void)
{
    vec2 p = (gl_FragCoord.xy / tres.xy) * 2.0 - 1.0;
    float a = atan(p.y, p.x);
    float r = sqrt(dot(p, p));
    vec2 uv = vec2(0.0, 0.0);
    uv.x = mod(par_a.x * cos(a) / r + time * 0.2, 1.0);
    uv.y = mod(par_a.y * sin(a) / r + time * 0.33, 1.0);
    
    float amount = sin(time * 0.1) * par_a.z;
    vec4 color0 = texture2D(tex[0], uv);
    vec4 color1 = texture2D(tex[0], uv + vec2(0.0, -amount));
    vec4 color2 = texture2D(tex[0], uv + vec2(0.0, amount));
    vec4 color3 = texture2D(tex[0], uv + vec2(amount, par_a.w));
    vec4 color4 = texture2D(tex[0], uv + vec2(-amount, -par_a.w));
    vec4 fragColor = ((color0 + color1 + color2 + color3 + color4) / 8.0) + pow(1.0 - r, 6.0);
    
    float border = 0.98;
    if (p.x < -border || p.x > border || p.y < -border || p.y > border)
{
        fragColor = vec4(p.x + p.y);
    }

    gl_FragColor = fragColor * color.a;
}