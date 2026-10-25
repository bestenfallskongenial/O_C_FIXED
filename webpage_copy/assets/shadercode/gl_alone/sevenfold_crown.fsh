precision mediump float;

// Sevenfold Crown
//
// An eight-power fractal forms a rounded central body surrounded by repeated lobes and finer branching edges. Rotation and scale reveal different portions of its crown, while a traveling brightness modulation passes over the otherwise fixed geometry.
//
// par_a.x - blends toward inverted colors while reducing brightness; halfway removes contrast.
// par_a.z - increases coordinate scale, revealing a wider portion of the fractal.
// par_a.w - rotates the field through a full turn.

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

vec2 cmul(vec2 a, vec2 b)
{
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 cpow8(vec2 z)
{
    vec2 z2 = cmul(z, z);
    vec2 z4 = cmul(z2, z2);
    return cmul(z4, z4);
}

float mandelbulb2D(vec2 c)
{
    vec2 z = c;
    float n = 0.0;
    for(int i = 0; i < 16; i++)
        {
        if(dot(z, z) > 4.0) break;
        z = cpow8(z) + c;
        n += 1.0;
        }
    return n;
}

void main()
{
    // Uniform mapping
    float adc5Min = 0.0;
    float adc5Max = 5.5;
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
    // Scale and offset
    uv *= 2.0;
    uv += vec2(-0.5, 0.0);
    // Calculate Mandelbulb-inspired fractal
    float n = mandelbulb2D(uv);
    // Normalize n
    n /= 100.0;
    // Color calculation
    vec3 color_out = 0.5 + 0.5 * cos(3.0 + n * 15.0 + vec3(0.0, 0.6, 1.0));
    // Add some variation based on time
    color_out *= 0.8 + 0.2 * sin(time * 5.0 + uv.xyx + vec3(0, 2, 4));
    // Apply effects based on uniforms
    color_out = mix(color_out, 1.0 - color_out, par_a[0]);  // Color inversion
    color_out *= 1.0 - (par_a[0] * 0.5);  // Darkness control
//    color_out = mix(color_out, vec3(dot(color_out, vec3(0.299, 0.587, 0.114))), par_a[1]);  // Saturation control

    gl_FragColor = vec4(color_out, color.a);
}