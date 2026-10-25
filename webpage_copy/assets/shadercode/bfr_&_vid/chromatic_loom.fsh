precision mediump float;

// CHROMATIC LOOM
//
// Two families of ribbons bend across one another, alternating above and below like a slowly turning woven fabric. Fine fibers and narrow highlights give the strands a rounded surface. vid textures the ribbons, while bfr leaves faint, contracting afterimages.
//
// par_a.x - ribbon density.
// par_a.y - bending strength and orientation of the weave.
// par_a.z - shifts ribbon hues and increasingly introduces video texture.
// par_a.w - strength of the backbuffer afterimages.

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

#define TAU 6.28318530718


vec3 pigment(float x)
{
    return 0.5 + 0.5 * cos(
        TAU * (
            vec3(0.02, 0.36, 0.69) + x
        )
    );
}

void main(void)
{
    vec2 uv = tcoord;

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / max(tres.y, 1.0);

    float density = mix(
        5.0, 19.0, par_a.x
    );

    float distortion = par_a.y;

    float t = time * 0.19;


    float angle =
        t * 0.11 +
        distortion * 0.8;

    float ca = cos(angle);
    float sa = sin(angle);

    vec2 q = vec2(
        ca * p.x - sa * p.y,
        sa * p.x + ca * p.y
    );

    q *= density;


    float wx =
        sin(q.y * 0.43 + t +
            sin(q.x * 0.18 - t) * 1.5);

    float wy =
        sin(q.x * 0.47 - t * 0.83 +
            cos(q.y * 0.21 + t) * 1.4);

    vec2 weave = q;

    weave.x += wx * distortion * 2.6;
    weave.y += wy * distortion * 2.6;


    vec2 id = floor(weave);

    vec2 local = fract(weave) - 0.5;

    float dx = abs(local.x);
    float dy = abs(local.y);


    float widthX =
        0.31 + 0.035 *
        sin(id.x * 1.7 + t);

    float widthY =
        0.31 + 0.035 *
        cos(id.y * 1.9 - t);

    float ribbonX = 1.0 - smoothstep(
        widthX - 0.025,
        widthX + 0.015,
        dx
    );

    float ribbonY = 1.0 - smoothstep(
        widthY - 0.025,
        widthY + 0.015,
        dy
    );


    float parity = mod(
        id.x + id.y,
        2.0
    );

    float xFront = 1.0 - parity;


    float nx = local.x /
        max(widthX, 0.001);

    float ny = local.y /
        max(widthY, 0.001);

    float curveX = sqrt(
        max(0.0, 1.0 - nx * nx)
    );

    float curveY = sqrt(
        max(0.0, 1.0 - ny * ny)
    );

    float lightX =
        0.22 +
        0.55 * curveX +
        0.23 * nx;

    float lightY =
        0.22 +
        0.55 * curveY -
        0.23 * ny;


    float hueX =
        id.x * 0.073 +
        sin(id.x * 0.37) * 0.19 +
        par_a.z * 1.5;

    float hueY =
        id.y * 0.091 +
        cos(id.y * 0.29) * 0.17 -
        par_a.z * 0.8;

    vec3 colX = pigment(hueX);
    vec3 colY = pigment(hueY);


    vec2 videoUV = clamp(
        uv + vec2(wx, wy) *
        distortion * 0.035,
        0.0,
        1.0
    );

    vec3 video = texture2D(
        vid,
        videoUV
    ).rgb;

    float videoAmount = smoothstep(
        0.25,
        1.0,
        par_a.z
    );

    colX = mix(
        colX,
        colX * (0.25 + video * 1.65),
        videoAmount
    );

    colY = mix(
        colY,
        colY * (0.25 + video.bgr * 1.65),
        videoAmount
    );


    float fiberX =
        sin(weave.y * 31.0 +
            id.x * 2.1);

    float fiberY =
        sin(weave.x * 31.0 -
            id.y * 2.3);

    colX *= lightX *
        (0.88 + 0.12 * fiberX);

    colY *= lightY *
        (0.88 + 0.12 * fiberY);


    float specX = pow(
        max(0.0, curveX * 0.8 +
            nx * 0.35),
        18.0
    );

    float specY = pow(
        max(0.0, curveY * 0.8 -
            ny * 0.35),
        18.0
    );

    colX += specX *
        vec3(0.65, 0.8, 1.0) * 0.55;

    colY += specY *
        vec3(1.0, 0.75, 0.55) * 0.55;


    vec3 background = vec3(
        0.008,
        0.012,
        0.025
    );


    vec3 result = background;

    if (xFront > 0.5)
    {
        // Y underneath, X above.

        result = mix(
            result,
            colY * 0.83,
            ribbonY
        );

        result = mix(
            result,
            colX,
            ribbonX
        );
    }
    else
    {
        // X underneath, Y above.

        result = mix(
            result,
            colX * 0.83,
            ribbonX
        );

        result = mix(
            result,
            colY,
            ribbonY
        );
    }


    float crossing =
        ribbonX * ribbonY;

    float contact =
        crossing *
        (1.0 - max(curveX, curveY));

    result *= 1.0 - contact * 0.4;


    vec2 historyUV =
        0.5 + (uv - 0.5) * 1.004;

    vec3 history = texture2D(
        bfr,
        clamp(historyUV, 0.0, 1.0)
    ).rgb;

    float memory = par_a.w;

    result = max(
        result,
        history * memory * 0.96
    );


    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
