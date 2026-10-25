precision mediump float;

// Cellular Tide
//
// Rounded diamonds stretch into cells, narrow channels, and irregular openings. Crossing wave fields continually rearrange their boundaries, making an orderly lattice loosen into organic forms before gathering together again.
//
// par_a.x - increases the reach of the time modulation, changing the pattern’s evolution.
// par_a.y - changes competing horizontal and vertical frequencies.
// par_a.z - shifts the threshold toward greater filled coverage.

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

float f0 = mix(0.05, 0.95, par_a[0]);   // lines 1
float f1 = mix(0.05, 0.95, par_a[1]);   // lines 2
float f2 = mix(0.05, 0.3, par_a[2]);    // shade

void main()
{
    vec2 position = gl_FragCoord.xy / tres.xy;
    float t = 80. * f0 * sin( time * 0.05);
    float color_out = 0.0;
    color_out = sin( position.x * cos( t / 166.0 ) * 20.0 ) + cos( position.y * cos( t / 165.0 ) * 10.0 );
    color_out *= sin( position.y * sin( t / 10.0 ) * 10.0 *f1 ) + cos( position.x * sin( t / 255.0 ) * 100.0 /f1 );
    color_out *= sin( position.x * sin( t / 5.0 ) * 10.0 ) + sin( position.y * sin( t / 105.0 ) * 120.0 );
    color_out -= sin( t / 100.0 ) * 0.8 - f2;
    color_out /= 0.01;
    color_out = smoothstep(color_out, 0.0, 25.5);
    gl_FragColor = vec4( vec3( color_out, color_out, color_out ), color.a );
}