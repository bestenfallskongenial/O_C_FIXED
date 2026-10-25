precision mediump float;

// Wave Cabinet
//
// Three traveling sine-wave traces cross inside an offset rectangular window. Around them, a noise-driven geometric field bends and shifts, separating the smooth motion of the enclosed curves from the restless surface of their surrounding frame.
//
// par_a.x - increases the red trace’s frequency, amplitude, and thickness while changing the surrounding field’s red and green mapping.
// par_a.y - increases the green trace’s frequency, amplitude, and thickness while changing the surrounding pattern’s intensity.
// par_a.z - increases the blue trace’s frequency, amplitude, and thickness while increasing the surrounding radial distortion.

varying vec2 tcoord;

uniform float time;
uniform vec2 tres;
uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;
uniform sampler2D tex[8];
uniform int tex_l;

// Parameters for first shader
float f0_1 = mix(0.1, 0.95, par_a[0]);
float f1_1 = mix(0.1, 0.95, par_a[1]);
float f2_1 = mix(0.1, 0.95, par_a[2]);

// Parameters for second shader
float f0_2 = mix(0.05, 0.95, par_a[0]);
float f1_2 = mix(0.05, 0.95, par_a[1]);
float f2_2 = mix(0.05, 0.95, par_a[2]);

// First shader functions
float drawGraph(vec2 pos, float env, float THICKNESS) {
    if ((pos.y < env + THICKNESS) && (pos.y > env - THICKNESS)) {
        return 1.0;
    }
    return 0.0;
}

// Second shader functions
vec2 Distort(vec2 p) {
    float theta = atan(p.y, p.x);
    float radius = length(p) * f2_2;
    radius = pow(radius, 1.3);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

vec4 pattern(vec2 p) {
    vec2 m = mod(p.xy + p.x + p.y, 2.0) - 1.0;
    return vec4(length(m + p * 0.1)) / f1_2;
}

float hash(const float n) {
    return fract(sin(n) * 43758.5453);
}

float noise(const vec3 x) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + p.z * 43.0;
    float r1 = mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
                   mix(hash(n + 57.0), hash(n + 57.0 + 1.0), f.x), f.y);
    float r2 = mix(mix(hash(n + 43.0), hash(n + 43.0 + 1.0), f.x),
                   mix(hash(n + 43.0 + 57.0), hash(n + 43.0 + 57.0 + 1.0), f.x), f.y);
    return mix(r1, r2, f.z);
}

// Window function
bool inWindow(vec2 pos) {
    return pos.x >= 0.3 && pos.x <= 0.9 && // 30% from left, 10% from right
           pos.y >= 0.1 && pos.y <= 0.9;   // 10% from top, 10% from bottom
}

void main() {
    vec2 normalizedPos = gl_FragCoord.xy / tres.xy;
    vec3 color_out;
    
    if (inWindow(normalizedPos)) {
        // First shader (inside window)
        vec2 position = normalizedPos * vec2(2.0) - vec2(1.0);
        float thickness = 0.1;
        
        color_out.x = drawGraph(position, sin(f0_1 * 16.0 * position.x + time * 8.0) * f0_1, thickness * f0_1);
        color_out.y = drawGraph(position, sin(f1_1 * 8.0 * position.x + time * 12.0) * f1_1, thickness * f1_1);
        color_out.z = drawGraph(position, sin(f2_1 * 12.0 * position.x + time * 16.0) * f2_1, thickness * f2_1);
    } else {
        // Second shader (outside window)
        vec2 position = normalizedPos * 8.0;
        float off = noise(position.xyx + time);
        vec4 c = pattern(Distort(position * off));
        c.xy = Distort(c.xy) * f0_2;
        color_out = vec3(c.x * off, pow(sin(c.y) - off, 0.5), pow(cos(c.z), 3.0));
    }
    
    gl_FragColor = vec4(color_out, color.a);
}