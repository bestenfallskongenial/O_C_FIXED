precision mediump float;

// Crosscurrent Shutters
//
// The source image bends through crossing waves while broad curved shutters sweep over it. Repeating folds expose small picture fragments between sweeping bands, making the image seem caught beneath a restless surface.
//
// par_a.x - rate of the evolving coordinate distortion.
// par_a.y - coordinate scale; higher values make source-image features smaller.
// par_a.z - rotation of the image mapping through a full turn.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers
uniform int tex_l;                      // number of loaded texures
     void main()
{
        float adc5Min = 2.0;
        float adc5Max = 4.0;
        float adc6Min = 0.0;
        float adc6Max = 360.0;
        float f0 = mix(0.05, 0.95, par_a[0]);
        float zoom       = adc5Min + par_a[1] * (adc5Max - adc5Min);
        float rotation   = adc6Min + par_a[2] * (adc6Max - adc6Min);

    vec2 uv = (gl_FragCoord.xy - tres) / min(tres.x, tres.y);
    uv *= zoom;
        float angle = radians(rotation);
    uv  = vec2(  uv.x * cos(angle) - uv.y * sin(angle), uv.x * sin(angle) + uv.y * cos(angle)   );
        float  time = time / tres.x;
        float variation = time * f0;
        float variationAngleA =  time  * 3.14159 *  variation;
        float variationAngleB =  time  * 3.14159 * -variation;
    uv += vec2(cos(variationAngleA + uv.y), sin(variationAngleB + uv.x));
    vec4 texColor1 = texture2D(tex[0], uv - vec2(0.5, 0.5));
    vec3 col    = 0.5 * tan(  time + uv.x + vec3(0.0, 0.0, 2.5));
         col   *= 0.5 * sin(- time + uv.y + vec3(3.0, 0.2, 0.2));
         col   *= 5.0 * cos(  time + uv.x + vec3(0.0, 0.0, 2.5));
    gl_FragColor = vec4(texColor1.rgb * col, color.a);
//  gl_FragColor = vec4(col, color.a);
}
