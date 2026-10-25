// // http://glslsandbox.com/e#41011.0
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

//f0:derivative:
//f1:zoom:
//f2:brightness:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.3, 0.95,  par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

float DERIV_DIF = f0;

struct surface_data {
    float depth;
    vec3 normal; };

float nsin(float x)
{ return (sin(x) + 1.0) / 2.0; }
float ncos(float x)
{ return (cos(x) + 1.0) / 2.0; }

float getDepth(vec2 p)
{
    p.x += sin(p.y * 12.0 + p.x + time * 2.0) * 0.01;
    p.y += cos(p.x + p.y + time) * 0.1;
    float l = length(p) * f1;
    float val = sin(l * 20.0 + time) + cos(l * 17.0 + time * 2.0) + sin(p.x * p.y + time);
    return val;
//p *= 6.;
//float a0 = sin(p.x * p.y/2.+ cos(p.x * p.y -  time));
//float a1 = sin(cos(p.x/2.) + sin(p.y));

//return 0.1*abs(a0 - a1);
}

surface_data getSurface(vec2 p)
{
    float depth = getDepth(p);
    return surface_data(
        depth, normalize(cross( vec3(DERIV_DIF, 0.0, getDepth(p + vec2(DERIV_DIF, 0.0)) - depth),
                                vec3(0.0, DERIV_DIF, getDepth(p + vec2(0.0, DERIV_DIF)) - depth)
        ))
    );
}

vec3 getColorAt(vec2 p)
{
    vec3 color_out = vec3(0.0, 0.0, 0.0);
    surface_data surface = getSurface(p);
    vec3 light_dir = normalize(vec3(1.0, 1.0, -1.0));
    vec3 light_pos = vec3(p + vec2(cos(time), sin(time)), 0.0);
    //vec3 light_pos = vec3((mouse-.5), 1.);
//light_pos.x *= tres.x / tres.y;
    color_out += vec3(1.0, 1.0, 1.0) * abs(dot(reflect(light_dir, surface.normal), light_pos));
    color_out += vec3(1.0, 1.0, 1.0) * dot(light_dir, surface.normal) * f2;
    //color_out += vec3(.2, .2, 0.2) * surface.depth;
//color_out += vec3(1, 0, 0) * surface.dx;
//color_out += vec3(0, 0, 1) * surface.depth;
    return color_out;
}

void main()
{
    vec2 pos = (gl_FragCoord.xy / tres) - 0.5;
    pos.x *= tres.x / tres.y;
    vec3 color_out = getColorAt(pos);
    gl_FragColor = vec4(color_out, color.a);
}