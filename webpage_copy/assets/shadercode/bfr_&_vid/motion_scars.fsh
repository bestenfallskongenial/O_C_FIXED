precision mediump float;

// MOTION SCARS
//
// Differences between vid and bfr leave displaced scars across the picture. Changing forms gather bright residues and lingering outlines, while older marks drift away from their source and accumulate into layered traces.
//
// par_a.x - sensitivity to differences between video and backbuffer.
// par_a.y - strength of the added scars.
// par_a.z - backbuffer persistence.
// par_a.w - horizontal and vertical displacement of the retained image.

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

    vec2 px = 1.0 / max(tres, vec2(1.0));


    vec3 live = texture2D(vid, uv).rgb;
    vec3 old  = texture2D(bfr, uv).rgb;


    vec2 offset = vec2(
        px.x * mix(0.0, 12.0, par_a.w),
        px.y * mix(0.0, 4.0, par_a.w)
    );

    vec3 displaced = texture2D(
        bfr,
        clamp(uv - offset, 0.0, 1.0)
    ).rgb;


    vec3 delta = abs(live - old);

    float difference = dot(
        delta,
        vec3(0.3333)
    );

    float motion = smoothstep(
        mix(0.25, 0.015, par_a.x),
        mix(0.45, 0.08, par_a.x),
        difference
    );


    vec3 residue = max(
        displaced - live * 0.35,
        vec3(0.0)
    );


    vec3 scar = vec3(
        residue.g,
        residue.b,
        residue.r
    );

    scar += delta.bgr * motion * 0.5;


    vec3 accumulated = mix(
        live,
        displaced,
        par_a.z * 0.97
    );

    accumulated +=
        scar * motion * par_a.y * 0.65;


    vec3 result = mix(
        accumulated,
        live,
        0.08 * (1.0 - par_a.z)
    );

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
