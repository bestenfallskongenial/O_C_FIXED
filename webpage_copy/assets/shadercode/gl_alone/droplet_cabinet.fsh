precision mediump float;

// Droplet Cabinet
//
// A rounded, flexing droplet moves inside an offset rectangular window. Its separated channel edges slide around the perimeter, while a distorted noise field surrounds the opening like a shifting frame around a suspended lens.
//
// par_a.x - increases the droplet’s apparent size and movement while changing the surrounding field’s red and green mapping.
// par_a.y - changes the spread of the droplet’s contributing centers and the surrounding pattern’s intensity.
// par_a.z - strengthens the droplet’s subtractive field while increasing the surrounding radial distortion.

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

// Parameters for first shader (metaballs)
float f0_1 = mix(0.1, 0.95, par_a[0]);
float f1_1 = mix(0.05, 0.95, par_a[1]);
float f2_1 = mix(0.05, 0.95, par_a[2]);

// Parameters for second shader (distorted noise)
float f0_2 = mix(0.05, 0.95, par_a[0]);
float f1_2 = mix(0.05, 0.95, par_a[1]);
float f2_2 = mix(0.05, 0.95, par_a[2]);

#define PI  3.14159265358979323846
#define TAU 6.28318530717958647692

// First shader functions (metaballs)
float random(in float val) {
    return fract(sin(val * 12.9898) * 43758.5453123);
}

float random(in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

vec2 random2(in float val) {
    vec2 st = vec2(dot(vec2(val), vec2(127.1, 311.7)), dot(vec2(val), vec2(269.5, 183.3)));
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

vec2 random2(in vec2 st) {
    st = vec2(dot(st, vec2(127.1, 311.7)), dot(st, vec2(269.5, 183.3)));
    return -1.0 + 2.0 * fract(sin(st) * 43758.5453123);
}

// Second shader functions (distorted noise)
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
        // First shader (metaballs inside window)
        float d = min(tres.x, tres.y) * 4.0 * f0_1 + 2.0;
        vec2 st = (gl_FragCoord.xy - (tres.xy / 2.0)) / d;
        vec2 p = (vec2(f0_1, f1_1) - (tres.xy / 2.0)) / d;
        
        vec3 m_dist = vec3(0.0);
        const int n = 4;
        for(int i = 0; i < n; i++) {
            vec2 pos = random2(vec2(float(i), 0.2)) * 0.2 * f1_1;
            float angle = mix(0.0, TAU, random(float(i)));
            float radius = mix(0.01, 0.2, random(float(i)));
            float freq = 1.0;
            if(random(float(i)) > 0.5) {
                freq = -1.0;
            }
            vec2 offset = radius * vec2(sin(freq * time + angle), cos(freq * time + angle));
            m_dist.r += 1.0 / distance(st + vec2(0.00, 0.00), pos + offset);
            m_dist.g += 1.0 / distance(st + vec2(0.01, 0.01), pos + offset);
            m_dist.b += 1.0 / distance(st + vec2(0.05, 0.00), pos + offset);
        }
        m_dist.r -= 8.0 * f2_1 / distance(st + vec2(0.00, 0.00), p);
        m_dist.g -= 8.0 * f2_1 / distance(st + vec2(0.01, 0.01), p);
        m_dist.b -= 8.0 * f2_1 / distance(st + vec2(0.05, 0.00), p);
        color_out = smoothstep(0.49, 0.5, m_dist * 0.01);
    } else {
        // Second shader (distorted noise outside window)
        vec2 position = normalizedPos * 8.0;
        float off = noise(position.xyx + time);
        vec4 c = pattern(Distort(position * off));
        c.xy = Distort(c.xy) * f0_2;
        color_out = vec3(c.x * off, pow(sin(c.y) - off, 0.5), pow(cos(c.z), 3.0));
    }
    
    gl_FragColor = vec4(color_out, color.a);
}