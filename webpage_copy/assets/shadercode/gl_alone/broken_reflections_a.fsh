precision mediump float;

// Broken Reflections
//
// Curved sheets buckle into sharp seams, repeating fragments, and narrow reflective-looking ridges. Their contours stretch and fold through one another, producing the restless movement of reflections broken across an uneven surface.
//
// par_a.x - changes the strength of the coordinate coupling, reshaping the folds.
// par_a.y - changes deformation within the repeated secondary field.
// par_a.z - stretches one shaping component while compressing the other.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:mod 1:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:mod 2:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:color pos:

void main()
{
    float t =  time * 2.0;
    float sint = sin( time*0.25);
    vec2 p = (-1.0 + 2.0*tcoord)*2.0*(1.0+sin(t/70.689)/3.5);
    vec2 p2 = abs(p * -2.5);
    vec2 p3 = p*p;
    float r = p.x*2.0 * f0; 
    float r2 = fract(length(p2));
    p.x = tan(clamp((p.x-r/2.0*cos(t/0.0)+p.y/r), -0.4, 0.4));
    p.y = tan(clamp((p.y-r/1.5*sin(t/0.5)+p.x/r), -0.55, 0.1));
    p2.x *= p.x * sin(-clamp((p2.x-r/0.5*cos(t/3.5)+p2.x/r), -0.4, 0.45)) / f1; 
    p2.y *= p.x * cos(clamp((-p2.y-r2/1.3*sin(sqrt(t)/2.4)+p2.y/r), -2.4, 0.3) /f1);
    p3.x = atan(clamp((-p3.x-r2/0.5*cos(t/2.1313)-p2.x/r2), -0.45, 0.3)) *f2;
    p3.y = atan(-clamp((p3.y-r/0.3*sin(t/1.414)+p3.y/r), -0.4, 0.4)) /f2;
    // Domain repetition mods:
    p2.x -= mod(p3.x,3.3) + 0.5;
    p2.y -= mod(p3.y,3.3) + 0.5;
    float b = (30.0/(sin(p2.x) - cos(p2.y))) / 1.5;
    float a = (1.0/(sin(p.x)  - cos(p.y) )) / 0.5;
    float c = (1.5/(cos(p.x) - sin(p2.y))) / 1.5;
    float d = (2.5/(cos(p.x) + sin(p2.y))) / 1.5 - (c/2.7);
    float f = (1.5/(cos(p3.x) + sin(p3.y))) / 1.5 + dot(b,a)/3.0;
    gl_FragColor = vec4(sqrt(2.0*a+b-c-d+f)/12.0-0.1, 
                        sqrt(2.0*a+b-c-d+f)/10.0-0.1,  
                        sqrt(1.8*a+b-c-d+f)/9.0-0.1,  
                        color.a);
    // Example of using textures if needed:
    // vec4 texColor1 = texture2D(tex[0], tcoord);
    // vec4 texColor2 = texture2D(tex[1], tcoord);
    // gl_FragColor *= texColor1;
}