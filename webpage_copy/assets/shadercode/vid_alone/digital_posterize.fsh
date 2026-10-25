precision mediump float;

// DIGITAL POSTERIZE
//
// vid breaks into stepped tonal regions, with a patterned grain stippling the transitions. The dither can grow into coarse cells and crawl sideways, giving the picture the restless surface of an animated print.
//
// par_a.x - posterization strength; reduces the available levels from 32 to 3.
// par_a.y - dither strength.
// par_a.z - size of the dither cells.
// par_a.w - horizontal stepping speed of the dither pattern.

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


    vec3 video = texture2D(vid, uv).rgb;


    float levels = floor(
        mix(32.0, 3.0, par_a.x)
    );


    float scale = mix(
        1.0, 8.0, par_a.z
    );

    vec2 pixel = floor(
        uv * tres / scale
    );

    pixel.x += floor(
        time * par_a.w * 8.0
    );


    vec2 cell = mod(pixel, 4.0);

    float row = cell.y;
    float col = cell.x;

    float dither =
        mod(
            col * 2.0
            + row * 3.0
            + mod(col + row, 2.0) * 7.0,
            16.0
        ) / 16.0;


    vec3 shifted = video +
        (dither - 0.5)
        * par_a.y
        / max(levels - 1.0, 1.0);

    vec3 result =
        floor(
            clamp(shifted, 0.0, 1.0)
            * (levels - 1.0) + 0.5
        ) / (levels - 1.0);

    gl_FragColor = vec4(
        result,
        color.a
    );
}
