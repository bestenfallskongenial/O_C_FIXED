precision mediump float;

// Signal Tear Memory
//
// Horizontal bands of a source texture slip out of alignment while bfr drifts in response to the same tears. Channel-swapped fragments enter the accumulating picture, and occasional hard clipping turns selected strips into stark digital interruptions.
//
// par_a.x - horizontal tear strength in the source and retained image.
// par_a.y - strength of additional broad-band source shifts.
// par_a.z - vertical displacement of selected backbuffer bands.
// par_b.x - backbuffer persistence.
// par_b.y - strength of fresh source imagery.
// par_b.z - strength of intermittent hard thresholding.

varying vec2 tcoord;

uniform float time;
uniform vec2  tres;

uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;

uniform sampler2D tex[8];
uniform int tex_l;

uniform sampler2D bfr;

float hash1(float n)
{
    return fract(sin(n * 12.9898) * 43758.5453);
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    // horizontal glitch bands

    float frame = floor(time * 12.0);
    float band  = floor(uv.y * 24.0);

    float rnd = hash1(band + frame * 7.13 + seed.x);

    float tear =
        step(0.72, rnd) *
        (rnd - 0.5) *
        mix(0.01, 0.22, par_a.x);

    // source texture

    vec2 suv = uv;

    suv.x += tear;

    // coarse horizontal block jumps
    float blockY = floor(uv.y * 12.0);
    float blockR = hash1(blockY * 4.7 + frame);

    suv.x +=
        step(0.84, blockR) *
        (blockR - 0.5) *
        par_a.y * 0.25;

    suv = clamp(suv, 0.0, 1.0);

    vec3 src = texture2D(tex[0], suv).rgb;

    // feedback gets a DIFFERENT tear

    vec2 fuv = uv;

    fuv.x -= tear * 0.35;

    // periodic vertical jump
    fuv.y +=
        step(0.90, rnd) *
        sin(time * 7.0 + band) *
        0.015 *
        par_a.z;

    fuv = clamp(fuv, 0.0, 1.0);

    vec3 old = texture2D(bfr, fuv).rgb;

    // cheap pseudo RGB glitch without extra texture reads

    float swapFlag =
        step(0.82,
             hash1(band + floor(time * 6.0)));

    vec3 glitchedSrc =
        mix(src, src.brg, swapFlag);

    // feedback

    float persistence =
        mix(0.82, 0.995, par_b.x);

    float injection =
        mix(0.01, 0.25, par_b.y);

    vec3 col =
        old * persistence +
        glitchedSrc * injection;

    // occasional hard digital clipping

    float clipGate =
        step(0.94,
             hash1(frame + band * 13.0));

    vec3 clipped =
        step(vec3(0.45), col);

    col = mix(col, clipped, clipGate * par_b.z);

    gl_FragColor =
        vec4(clamp(col, 0.0, 1.0), color.a);
}