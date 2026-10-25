precision mediump float;

// Tile Blitter
//
// Tiles of bfr hop horizontally and vertically, their directions guided by brightness in a source texture. Fragments wrap around the frame, while a central vertical strip introduces fresh imagery into the rearranging mosaic.
//
// par_a.x - tile density.
// par_a.y - rate at which tile directions cycle.
// par_a.z - hop distance, from 1 to 8 tiles.
// par_a.w - width of the central strip introducing fresh source imagery.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Screen resolution

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];
uniform sampler2D bfr;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;

    float tiles_x = floor(mix(12.0, 96.0, par_a[0]));
    float tiles_y = max(1.0, floor(tiles_x * tres.y / tres.x));

    vec2 grid = vec2(tiles_x, tiles_y);

    vec2 cell = floor(uv * grid);
    vec2 frac_uv = fract(uv * grid);
    vec2 cell_uv = (cell + 0.5) / grid;

    vec4 ctl = texture2D(tex[0], cell_uv);
    float lum = dot(ctl.rgb, vec3(0.299, 0.587, 0.114));

    float phase = fract(lum * 4.0 + time * mix(0.05, 0.80, par_a[1]));

    vec2 dir = vec2(0.0, 0.0);

    if (phase < 0.25)
        {
        dir = vec2( 1.0, 0.0);
        }
    else if (phase < 0.50)
        {
        dir = vec2(-1.0, 0.0);
        }
    else if (phase < 0.75)
        {
        dir = vec2(0.0,  1.0);
        }
    else
        {
        dir = vec2(0.0, -1.0);
        }

    float hop = floor(mix(1.0, 8.0, par_a[2]));

    vec2 src_cell = mod(cell - dir * hop + grid, grid);
    vec2 src_uv = (src_cell + frac_uv) / grid;

    vec4 old = texture2D(bfr, src_uv);

    float head_w = mix(0.02, 0.20, par_a[3]);
    float head = 1.0 - step(head_w * 0.5, abs(uv.x - 0.5));

    vec4 fresh = texture2D(tex[0], uv);

    vec3 rgb = mix(old.rgb, fresh.rgb, head);

    gl_FragColor = vec4(rgb, color.a);
}