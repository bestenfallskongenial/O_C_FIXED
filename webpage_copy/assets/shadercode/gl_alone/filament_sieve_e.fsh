precision mediump float;

// Moire Veil
//
// Interlocking repetitions form a dense mesh of fine marks and broken lines. Slowly changing phases disturb the mesh, making delicate bands shimmer through broader bright regions like a veil drawn across another patterned surface.
//
// par_a.x - increases horizontal pattern frequency.
// par_a.y - increases vertical pattern frequency.
// par_a.z - reduces red intensity while raising the green-channel baseline.
// par_a.w - increases coordinate scale and rotates the pattern simultaneously.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:horz stretch:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert stretch:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:color_out changes:
float f3 = mix(0.5, 2.0, par_a[3]);
float f4 = radians(mix(0.0, 360.0, par_b[0]));
//just testing and learning :)
void main( void )
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
    uv *= f3;                           // UV transformation (zoom)
    
    uv = vec2(uv.x * cos(f3) - uv.y * sin(f3), uv.x * sin(f3) + uv.y * cos(f3)); // UV transformation (rotation)
              
    vec2 position = (uv + 1.0) * 0.5 * tres * vec2(f0, f1) * 8.;    // Original code starts here
    float depth = gl_FragCoord.z;
    float color_out = 0.5;
    float my_mod = 5.;
    //color_out += mod(time, 0.1);
    //color_out += dot(position.x, position.y)*depth;
    color_out *= abs(mod(position.x*my_mod*sin(position.x)*position.y+cos(time), position.x*my_mod*cos(position.x)*position.x+sin(time)));
    color_out *= abs(mod(position.y*my_mod*cos(position.y)*position.x+sin(time), position.y*my_mod*sin(position.y)*position.y+cos(time)));
    
    vec3 finalColor = vec3(color_out*0.2 / f2, color_out*0.1 + f2, color_out);
    
    gl_FragColor = vec4(finalColor, color_out.a);
}