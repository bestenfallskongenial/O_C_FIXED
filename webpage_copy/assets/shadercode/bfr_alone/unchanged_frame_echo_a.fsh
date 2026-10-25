precision mediump float;

// Unchanged Frame Echo
//
// The previous picture passes straight from bfr into the next frame. Whatever image has entered the buffer remains suspended, creating a still echo of the retained canvas.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D bfr;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;
    gl_FragColor = texture2D(bfr, uv);
    gl_FragColor.a = color.a;
}