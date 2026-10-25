precision mediump float;

// Articulated Shards
//
// Rotating square-edged forms intersect and fold into segmented wedges. Their overlaps create shifting dark gaps and narrow textured surfaces, suggesting articulated shards turning around a shared center.
//
// par_a.x - increases rotation speed.
// par_a.y - selects rotation direction: below halfway reverses it.
// par_a.z - changes the size of the square-shaped masks and their overlap.

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
    float c = cos(a), s = sin(a);    // Removed the 0.1 and 0.5 offsets for smoother rotation
    return mat2(c,s,-s,c);
}

vec2 pmod(vec2 p, float r){
    float a = atan(p.x,p.y)+PI/r;
    float n = PI2/r;
    a = floor(a/n)*n;
    return p*rot(a);    // Removed the 2.+1.* multiplier for more controlled rotation
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
    vec2 d = abs(p) - vec2(par_a.z);   // Using par_a.z to control actual cross width
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float sphere(vec2 p){
    return length(0.2/p)-5.0;
}

void main(void) {
    vec2 p = (gl_FragCoord.xy*2.0 - tres.xy)/min(tres.x,tres.y);
    vec2 z = p;
    
    // Smooth rotation control using par_a.x and par_a.y
    float direction = par_a.y < 0.5 ? -1.0 : 1.0;
    float rotSpeed = par_a.x * 2.0;    // Scaled for better control
    p *= rot(time * direction * rotSpeed);
    
    p = pmod(p,2.1);
    vec3 box = vec3(sdbox(0.7*p));

    p.x += 0.8*sin(time);
    vec3 box2 = vec3(sdbox(p));
    vec3 s = vec3(sphere(z));
    box = min(max(box,-box2),-s);
    
    vec3 c = vec3(sin(time),cos(time),rand(rand(time)));
    vec3 col = vec3(noise(p));

    gl_FragColor = vec4(box*col,color.a);
}