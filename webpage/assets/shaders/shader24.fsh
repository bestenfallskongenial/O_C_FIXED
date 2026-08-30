// user10 Electric Field Visualization                                                                                                              
//--------------------------// shader name length of 26 characters

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

const float PI = 3.14159265359;

vec2 field(vec2 p)
{
    vec2 force = vec2(0.0);
    for(int i = 0; i < 5; i++)
{
        float t = time * (0.1 + float(i) * 0.05) + float(i) * PI / 2.5;
        vec2 center = vec2(sin(t), cos(t)) * 0.5;
        float charge = sin(t * 2.0) * 0.5 + 0.5;
        vec2 diff = p - center;
        float dist = length(diff);
        force += normalize(diff) * charge / (dist * dist + 0.001);
    }
    return force;
}

void main()
{
    vec2 uv = (tcoord - 0.5) * 2.0;
    
    vec2 f = field(uv);
    float mag = length(f);
    vec2 norm = normalize(f);
    
    float angle = atan(norm.y, norm.x);
    
    vec3 col = 0.5 + 0.5 * cos(angle + vec3(0.0, 2.0, 4.0) + time * par_a[0]);
    col *= smoothstep(0.0, 0.2, mag);
    
    vec2 flow = uv + f * (sin(time * par_a[1] * 0.001 ) * 0.5 + 0.5);
//    vec3 texCol = mix(texture2D(tex[0], flow * 0.5 + 0.5).rgb, 
//                      texture2D(tex[1], flow * 0.5 + 0.5).rgb, par_a[1]);
    vec3 texCol = texture2D(tex[0], flow * 0.5 + 0.5).rgb;
    col = mix(col, texCol, par_a[2]);
    
    gl_FragColor = vec4(col, color.a );
}