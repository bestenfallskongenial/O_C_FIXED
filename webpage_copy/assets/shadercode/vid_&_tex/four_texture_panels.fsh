precision mediump float;

// Four Texture Panels
//
// Four source textures stand side by side in equal vertical panels, each picture squeezed into its own narrow window. Together they form a strip of four simultaneous views.

#extension GL_OES_EGL_image_external : require  // new

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];

uniform samplerExternalOES vid;                 // new

void main()
{
    float column = floor(tcoord.x * 4.0);
    vec2 uv = vec2(fract(tcoord.x * 4.0), tcoord.y);
    vec4 sampled;

    if (column < 1.0)
        sampled = texture2D(tex[0], uv);
    else if (column < 2.0)
        sampled = texture2D(tex[1], uv);
    else if (column < 3.0)
        sampled = texture2D(tex[2], uv);
    else if (column < 4.0)
       sampled = texture2D(tex[3], uv);
//  else if (column < 5.0)
//      sampled = texture2D(tex[4], uv);
//  else if (column < 6.0)
//      sampled = texture2D(tex[5], uv);
//  else if (column < 7.0)
//      sampled = texture2D(tex[6], uv);
    else 
//      sampled = texture2D(tex[7], uv);
        sampled = texture2D(vid, uv);    

    gl_FragColor = vec4(sampled.rgb, color.a);      // new
}