precision mediump float;

// FRAME DRIFT
//
// A traveling wave bends vid sideways, making the picture sway in broad horizontal folds. Slightly displaced bfr copies linger behind it, softening movement into a drifting echo.
//
// par_a.x - horizontal bending strength.
// par_a.y - wave speed.
// par_a.z - strength and displacement of the backbuffer echo.

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

    float bend  = par_a.x;
    float speed = par_a.y;
    float echo  = par_a.z;


    float wave = sin(
        uv.y * 12.0
        + time * (0.2 + speed * 3.0)
    );

    vec2 shifted = uv;

    shifted.x += wave * bend * 0.08;


    vec3 video = texture2D(
        vid,
        clamp(shifted, 0.0, 1.0)
    ).rgb;


    vec2 oldUV = uv;

    oldUV.x += wave * 0.003 * echo;
    oldUV.y += 0.002 * echo;

    vec3 previous = texture2D(
        bfr,
        clamp(oldUV, 0.0, 1.0)
    ).rgb;


    vec3 result = mix(
        video,
        previous,
        echo * 0.92
    );

    gl_FragColor = vec4(result, color.a);
}
