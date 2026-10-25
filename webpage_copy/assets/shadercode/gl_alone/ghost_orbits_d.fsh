precision mediump float;

// Three Light Loops
//
// Three moving curve fields gather around a bright central region. Their fine loops stretch, overlap, and separate, leaving faint sweeping arcs like luminous threads circling a small knot.
//
// par_a.x - changes horizontal motion of the first curve field.
// par_a.y - changes horizontal motion of the second curve field.
// par_a.z - changes horizontal motion of the third curve field.

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
//f0:circle one:
//f1:circle two:
//f2:circle three:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

const float PI = 3.14159265;

vec3 sim(vec3 p, float s) {
    vec3 ret = p;
    ret = p + s / 2.0;
    ret = fract(ret / s) * s - s / 2.0;
    return ret;
}

vec2 rot(vec2 p, float r) {
    vec2 ret;
    ret.x = p.x * cos(r) - p.y * sin(r);
    ret.y = p.x * sin(r) + p.y * cos(r);
    return ret;
}

vec2 rotsim(vec2 p, float s) {
    vec2 ret = p;
    ret = rot(p, -PI / (s * 2.0));
    ret = rot(p, floor(atan(ret.x, ret.y) / PI * s) * (PI / s));
    return ret;
}

vec2 makeSymmetry(vec2 p) {
    vec2 ret = p;
    ret = rotsim(ret, sin(time * 0.1) * 0.0 + 1.0);
    ret.x = abs(ret.x);
    return ret;
}

float makePoint(float x, float y, float fx, float fy, float sx, float sy, float t) {
    float xx = x + tan(t * fx);
    float yy = y - tan(t * fy) * sy;
    return 0.8 / sqrt(abs(x * xx + y * yy));
}

void main(void) {
    vec2 p = (gl_FragCoord.xy / tres.x) * 2.0 - vec2(1.0, tres.y / tres.x);
    p = p * 10.0;
    
    float x = p.x * 0.4; 
    float y = p.y * 0.4;
    
    float t = sin(time * 0.4);
    float c = makePoint(x, y, f0 * 10.0, 0.0, 0.0, 0.0, t);
    c = c + makePoint(x, y, f1 * 3.0, 1.3, 0.4, 0.4, t);
    c = c + makePoint(x, y, f2 * 5.3, 0.5, 0.5, 0.5, t);
    
    vec3 d = vec3(c, c, c) / 20.0;
    gl_FragColor = vec4(d.rgb, color.a);
}