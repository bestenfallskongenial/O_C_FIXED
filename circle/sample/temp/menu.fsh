precision mediump float;

varying vec2 tcoord;

uniform sampler2D u_menu_atlas;
uniform int u_menu_tile_count;
uniform vec4 u_menu_tile_rect[20];
uniform float u_menu_tile_index[20];

const float kAtlasGrid = 8.0;

void main()
{
    vec4 out_color = vec4(0.0, 0.0, 0.0, 0.0);

    for (int i = 0; i < 20; ++i)
    {
        if (i >= u_menu_tile_count)
        {
            break;
        }

        vec4 rect = u_menu_tile_rect[i];

        float in_x = step(rect.x, tcoord.x) * step(tcoord.x, rect.x + rect.z);
        float in_y = step(rect.y, tcoord.y) * step(tcoord.y, rect.y + rect.w);

        if (in_x * in_y > 0.5)
        {
            float local_x = (tcoord.x - rect.x) / rect.z;
            float local_y = (tcoord.y - rect.y) / rect.w;

            float idx = floor(u_menu_tile_index[i] + 0.5);
            float tile_x = mod(idx, kAtlasGrid);
            float tile_y = floor(idx / kAtlasGrid);

            vec2 uv = vec2((tile_x + local_x) / kAtlasGrid,
                           (tile_y + local_y) / kAtlasGrid);

            vec4 tile_color = texture2D(u_menu_atlas, uv);
            out_color = mix(out_color, tile_color, tile_color.a);
        }
    }

    gl_FragColor = out_color;
}