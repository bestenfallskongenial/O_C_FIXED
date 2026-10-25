precision mediump float;

// Fractured Vanes
//
// Angular folded regions rotate through a broken, grainy surface. Box-shaped masks cut across one another, revealing narrow ridges and broad vanes that slide around a cross-like central division.
//
// par_a.x - increases rotation speed.
// par_a.y - selects rotation direction: below halfway reverses it.
// par_a.z - changes the width and overlap of the box-shaped masks.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // x = rotation speed, y = direction, z = cross width
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers
uniform int tex_l;                      // number of loaded texures

vec2 mouse = par_a.xy;

const float PI = acos(-1.);
float PI2 = PI*tan(12.);
		      
mat2 rot(float a){
    float c = cos(0.1+a),s = sin(0.5-a);
    return mat2(c,s,-s,c);
}

vec2 pmod(vec2 p,float r){
    float a = atan(p.x,p.y)+PI/r;
    float n = PI2/r;
    a = floor(a/n)*n;
    return p*rot(2.+1.*a);
}

float rand(float n){
    return fract(sin(.2*n)*943.321651);
}

float rand(vec2 p){
    return rand(dot(p.xy,vec2(31.3214,5.5432)));
}

float noise(vec2 p){
    vec2 i = floor(sin(tan(p)));
    vec2 f = fract(tan(p));
    
    vec2 u = f*f*(3.0-2.0*f);
    
    float a = rand(i);
    float b = rand(i + vec2(1.0,0.0));
    float c = rand(i + vec2(0.0,1.0));
    float d = rand(i + vec2(1.0,1.0));
    
    float n = mix(mix(mix(a,b,u.x),mix(c,d,u.x),u.x),(mix(c,d,u.x),mix(a,b,u.y),u.y),p.x*p.y);
    
    return n;
}

float sdbox(vec2 p){
    p=abs(p);
    float b =(max(p.x,p.y)-par_a.z);  // Using par_a.z to control cross width
    return b;
}

float sphere(vec2 p){
    return length(0.2/p)-5.0;
}

void main(void) {
    vec2 p = (gl_FragCoord.xy*2.0- tres.xy)/min(tres.x,tres.y);
    vec2 z = p;
    
    float direction = (par_a.y < 0.5) ? -1.0 : 1.0;
    p *= rot(time * par_a.x * direction);  // Control rotation speed and direction
    
    p = pmod(p,2.1);
    vec3 box = vec3(sdbox(.7*p));

    p.x += 0.8*sin(time);
    vec3 box2 = vec3(sdbox(p));
    vec3 s = vec3(sphere(z));
    box = min(max(box,-box2),-s);
    
    vec3 c = vec3(sin(time),cos(time),rand(rand(time)));
    vec3 col = vec3(noise(p));

    gl_FragColor = vec4(box*col,color.a);
}