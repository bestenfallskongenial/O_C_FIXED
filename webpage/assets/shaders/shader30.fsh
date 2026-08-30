// who knows?!                                 
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

uniform sampler2D tex[8];               // Array of tex samplers  uniform int tex_l;     
void main(void)
{
         vec2 o_trn = (gl_FragCoord.xy - tres.xy) / tres.y;
        float n_min = 1.0;
    vec3 o_col = vec3(1.0);
        // Keep all existing controls:
    float timeScale = mix(0.1, 2.0, par_a[2]);
    float patternScale = mix(0.1, 1.0, par_a[3]);
        // Map mouse to ADCs (as an addition to existing controls, not replacing them)
    // Using par_a[1], par_a[2] to simulate mouse x,y
    vec2 virtual_mouse = vec2(par_a[0], par_a[1]);
        const float n_tau = 6.2831;
             for(int i = 0; i < 30; i++)
{
        float n_it = float(i);
        float n_it_nor = n_it / 30.0;
       float sinval = sin(n_it_nor * n_tau);
        float cosval = cos(n_it_nor * n_tau);
       // Keep original controls AND add mouse simulation
        vec2 o_trn2 = vec2(sinval, cosval) *             sin( time * timeScale + n_it_nor * n_tau * 99.0 *  time * virtual_mouse.x * 0.01) *             patternScale * 0.45;
       float n_d = length(o_trn2 - o_trn);
        n_d = pow(n_d, 1.0/((n_it_nor + 0.5) * 10.0));
       if(n_d < n_min)
{
            o_col = vec3(n_it_nor);
            n_min = n_d;
        }
    }
        vec3 finalColor = sqrt(1.0 - n_min) * o_col;
    gl_FragColor = vec4(finalColor, color.a );
}