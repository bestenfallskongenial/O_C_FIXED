// Transposed Kaleidoscope Shader               
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

//f0:x mod:
//f1:y mod:
//f2:rotate:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.0, 1.0, par_a[2]);

mat2 rotate(float a)
{
    float c = cos(a), s = sin(a);
    return mat2(c, -s, s, c);
}

void main()
{
    vec2 uv = (2.0 * gl_FragCoord.xy - tres) / tres.y;
    vec3 color_out = vec3(0.9);
    
    uv *= rotate(f2 * 6.28319);
    uv = abs(uv);
    
    vec2 ar = vec2(sin(atan(uv.x, uv.y)), length(uv)) * vec2(f0, f1);
    
    color_out = mix(color_out, vec3(0.4, 0.0, 0.7), sin(8.0 * (ar.y * 1.0 - time) + time) + (8.0 * ar.x - 8.0 * ar.y));
    color_out = mix(color_out, vec3(0.7, 0.2, 0.0), cos(8.0) * (5.0 * ar.y - 8.0 * ar.x));
    color_out /= mix(color_out, vec3(0.4, 0.0, 0.7), sin(8.0 * (ar.y * 1.0 - time) + time) + (8.0 * ar.x - 8.0 * ar.y));
    
    gl_FragColor = vec4(color_out, color.a);
}