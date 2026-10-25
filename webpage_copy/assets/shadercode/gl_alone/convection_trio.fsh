precision mediump float;

// Convection Trio
//
// Three offset noise layers form rounded islands that swell, merge, and drift through one another. Their overlapping silhouettes create the slow circulation of a lava lamp, with separate layers revealing different parts of the moving field.
//
// par_a.x - blends the original channel arrangement toward a cyclic exchange of red, green, and blue.

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

float f0 = mix(0.1, 0.95, par_a[0]);    // f0: :
float f1 = mix(0.05, 0.95, par_a[1]);   // f1: :
float f2 = mix(0.05, 0.95, par_a[2]);   // f2: :

// Simplex noise function
vec3 mod289(vec3 x)
{ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x)
{ return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x)
{ return mod289(((x*34.0)+1.0)*x); }

float snoise(vec2 v)
{
    const vec4 C = vec4(0.211324865405187, 0.366025403784439,
                        -0.577350269189626, 0.024390243902439);
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v -   i + dot(i, C.xx);
    vec2 i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod289(i);
    vec3 p = permute(permute(i.y + vec3(0.0, i1.y, 1.0)) + i.x + vec3(0.0, i1.x, 1.0));
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
    m = m*m; m = m*m;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0 + h*h);
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
    uv *= 0.3;

    // First blob layer
    float n1 = snoise(uv * 3.0 + time * vec2(0.1, 0.2));
    float n2 = snoise(uv * 2.0 - time * vec2(0.15, 0.25));
    float blob1 = smoothstep(0.2, 0.3, n1 * 0.5 + n2 * 0.5);
    vec3 color1 = vec3(0.9, 0.3, 0.1) * blob1;  // Red blob

    // Second blob layer
    float n3 = snoise((uv + vec2(1.0, 0.0)) * 3.0 + time * vec2(0.1, 0.2));
    float n4 = snoise((uv + vec2(1.0, 0.0)) * 2.0 - time * vec2(0.15, 0.25));
    float blob2 = smoothstep(0.2, 0.3, n3 * 0.5 + n4 * 0.5);
    vec3 color2 = vec3(0.1, 0.9, 0.3) * blob2;  // Green blob

    // Third blob layer
    float n5 = snoise((uv + vec2(0.0, 1.0)) * 3.0 + time * vec2(0.1, 0.2));
    float n6 = snoise((uv + vec2(0.0, 1.0)) * 2.0 - time * vec2(0.15, 0.25));
    float blob3 = smoothstep(0.2, 0.3, n5 * 0.5 + n6 * 0.5);
    vec3 color3 = vec3(0.3, 0.1, 0.9) * blob3;  // Blue blob

    // Combine colors
    vec3 color_out = color1 + color2 + color3;
    
    // Apply color inversion based on par_a[0]
    //color_out = mix(color_out, 1.0 - color_out, par_a[0]);
    // Rotate colors by f0
    color_out = mix(color_out, 
                vec3(color_out.g, color_out.b, color_out.r), 
                f0);
    gl_FragColor = vec4(color_out, color.a);
}