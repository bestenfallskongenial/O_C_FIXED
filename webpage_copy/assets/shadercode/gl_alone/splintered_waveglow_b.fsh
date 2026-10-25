precision mediump float;

// Shattered Waveledge
//
// A wavering ledge breaks into luminous slivers, small arches, and scattered reflections. Its fragments tremble across the field, briefly gathering into ridges before falling apart again.
//
// par_a.x - horizontal wave frequency and fold shaping.
// par_a.y - phase of the vertical distortion.
// par_a.z - vertical shaping and glow intensity.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:horz wave stretch:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert wave stretch:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:glow brightness:

float t;

float f(float x, float y)
{
    x = (f0 * 10.0) * x; //Interactive
    float nx = x + cos(t * x);
    float ny = y + sin(t * y + f1 * 100.0);
    float formula = 1.0 / (sin(x + t + (2.0 * cos(t))) - y * sin(t) - sin(t / sqrt(t + ny * nx)) - tan(sqrt(f0)));
    float formula_effects = formula / asin(ny * f2);
    return formula_effects;
}

void main()
{
    t = 10.0 * sin(time);
    // CONTROLS
    float zoom = 10.0;
    float cameraX = (zoom * 5.0) / 10.0;
    float cameraY = (zoom * 3.0) / 10.0 - 0.5;
    
    zoom += 0.5;
    vec2 p = (gl_FragCoord.xy / (tres.x * 1.0 / zoom));
    float x = p.x - cameraX;
    float y = p.y - cameraY;
    float a = f(x, y);
    
    vec3 color_out = vec3(  sqrt(a + (3.5 * (sin(t)))) / 10.0 * sqrt(x * cos(t * x)),
                            sqrt(a) / 8.0 * sqrt(y + (sin(t) * tan(time * 0.01))),
                            sqrt(a) / 9.0 * sqrt(cos(t * 10.0) + y + sin(t * y))        );
    
    gl_FragColor = vec4(color_out, color.a);
}