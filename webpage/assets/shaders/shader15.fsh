// https://www.glslsandbox.com/e#63901.0         
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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:radial color:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:zoom:
float f2 = mix(0.65, 0.95, par_a[2]);   //f2:center converge:

vec2 mouse = par_a.xy;

float m(vec3 p)
{ 
    p.z += time/1.; 
     return abs(dot(p = cos(p*0.66 + sin(p.zxy*1.8)), p) - 1.1);
}

void mainImage( out vec4 c, vec2 u)
{
    vec3 d = vec3(u.x/tres.x-0.5, u.y/tres.y - 0.5, 0.5), o;
    
    float j, i = 0.;
    
    for (int i=0; i<6;++i) 
        j = m(o)/3.,
        c -= j<.01 ? 256.*(j - mouse.x)*j/o.z : 0.,
        o+= max(mouse.y, j)*d;
}

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}