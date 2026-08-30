// Circuit Tunnel Shader (based on shadertoy.com/view/MdXXW2)
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

uniform sampler2D tex[8];               // Array of texture samplers       
uniform int tex_l;                      // number of loaded texures             
vec2 rotate(vec2 p, float a)
{
    return vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
}

#define ITS 4  // Increased from 2

vec2 circuit(vec3 p)
{
    p = mod(p, 2.0) - 1.0;
    float w = 1e38;
    vec3 cut = vec3(1.0, 0.0, 0.0);
    vec3 e1 = vec3(-1.0);
    vec3 e2 = vec3(1.0);
    float rnd = 0.23;
    float pos, plane, cur;
    float fact = 0.9;
    float j = 0.0;
    for(int i = 0; i < ITS; i++)
    {
        pos = mix(dot(e1, cut), dot(e2, cut), (rnd - 0.5) * fact + 0.5);
        plane = dot(p, cut) - pos;
        if(plane > 0.0)
        {
            e1 = mix(e1, vec3(pos), cut);
            rnd = fract(rnd * 9827.5719);
            cut = cut.yzx;
        }
        else
        {
            e2 = mix(e2, vec3(pos), cut);
            rnd = fract(rnd * 15827.5719);
            cut = cut.zxy;
        }
        j += step(rnd, 0.2);
        w = min(w, abs(plane));
    }
    return vec2(j / float(ITS - 1), w);
}

float nse(float x)
{
    return fract(sin(x * 297.9712) * 90872.2961);
}

float scene(vec3 p) 
{
    vec2 cir = circuit(p);
    float edge = -100.0 * cir.y;           
    edge = edge > -10.0 ? exp(edge) : 0.0; 
    
    float anim = p.z * 10.0 + time * -5.0 + cir.x * 10.0;
    anim = sin(anim) * 0.5 + 0.5;         
    anim *= cir.x * 1.8;
    anim = anim * anim * anim;            // Simpler than pow(anim, 8.0)
    
    return edge + anim;
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;
    vec2 suv = uv;
    uv = 2.0 * uv - 1.0;
    uv.x *= tres.x / tres.y;
    vec3 ro = vec3(0.0, time * 0.2, 0.1);
    vec3 rd = normalize(vec3(uv, 1.));
    ro.xz = rotate(ro.xz, time/3.);
    ro.xy = rotate(ro.xy, 0.1);
    rd.xz = rotate(rd.xz, time*.25);
    rd.xy = rotate(rd.xy, 0.1);
    float acc = 0.0;
    vec3 r = ro + rd * 0.9;
    
    for(int i = 0; i < 5; i++)  // Increased from 3
    {
        acc += scene(r + nse(r.x) * 0.03);
        r += rd * 0.015;
    }
    
    vec3 col = pow(vec3(acc * .03), vec3(.1,.1,.1) * 13.0) * 2.3;
    col -= exp(length(suv - 0.5) * -2.5 - 0.2);
    col = clamp(col, vec3(0.0), vec3(1.0));
    col = pow(col, vec3(1.0 / 2.2));
    gl_FragColor = vec4(col, color.a);
}