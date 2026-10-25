precision mediump float;

// Phase Interference
//
// Waves from two moving centers cross into curved fringes and finely divided interference bands. Bright seams gather between dark cancellation lines, continually rearranging as the sources wander around one another.
//
// par_a.x - wave density.
// par_a.y - strength and density of the secondary interference modulation.
// par_a.z - wave propagation and source movement speed.
// par_a.w - blends from a uniform tint to spectral coloring.

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

uniform sampler2D bfr;
uniform sampler2D vid;


void main(void)
{
    vec2 uv = tcoord;

    float aspect = tres.x / max(tres.y, 1.0);

    vec2 p = uv * 2.0 - 1.0;
    p.x *= aspect;

    float density = par_a.x;
    float complexity = par_a.y;
    float speed = par_a.z;
    float chroma = par_a.w;


    float t = time * (0.1 + speed * 1.8);

    vec2 sourceA = vec2(
        sin(t * 0.41) * 0.65,
        cos(t * 0.37) * 0.55
    );

    vec2 sourceB = vec2(
        cos(t * 0.29) * 0.75,
        sin(t * 0.53) * 0.6
    );


    float freq = mix(8.0, 65.0, density);

    float a = length(p - sourceA) * freq - t * 4.0;
    float b = length(p - sourceB) * freq - t * 3.7;


    float waveA = sin(a);
    float waveB = sin(b);

    float interference = (waveA + waveB) * 0.5;


    float secondary = sin(
        (a - b) * (1.0 + complexity * 3.0)
    );

    float field = mix(
        interference,
        interference * secondary,
        complexity
    );


    float intensity = 1.0 - abs(field);

    intensity = intensity * intensity;
    intensity = intensity * intensity;


    vec3 spectral = vec3(
        0.5 + 0.5 * sin(a * 0.25),
        0.5 + 0.5 * sin(b * 0.25 + 2.1),
        0.5 + 0.5 * sin((a + b) * 0.125 + 4.2)
    );

    vec3 monochrome = vec3(
        0.75, 0.85, 1.0
    );

    vec3 result = mix(
        monochrome,
        spectral,
        chroma
    ) * intensity;


    float nodes = abs(waveA - waveB);

    result *= smoothstep(
        0.0,
        0.25,
        nodes
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
