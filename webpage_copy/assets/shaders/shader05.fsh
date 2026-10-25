// tex-pack user13                                                 
//----------------------------------------// shader name length of 40 characters

precision mediump float;
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
     
   float f0 = mix(0.05, 0.95, par_a[0]);    //f0:x pos:
   float f1 = mix(0.05, 0.95, par_a[1]);    //f1:y pos:
   float f2 = mix(0.05, 0.95, par_a[2]);    //f2:bright radius:

void main(void)
{
    float t =  time;

    vec2 p = gl_FragCoord.xy; 
    p /= tres.xy;

    vec4 col = vec4(1.0);
        col.rgb     = vec3( texture2D(tex[0], p).r, 
                            texture2D(tex[0], p - 0.025 + cos(t + p.y + p.x)*0.1).g, 
                            texture2D(tex[0], p + 0.04  + sin(t + p.y + p.x)*0.1).b );
        col.rgb    *= 1.75;
        float a     = t*0.25;
        col.gb     -= texture2D(tex[0], (p + length(p) ) * mat2(cos(a),
                     -sin(a),                         
                      sin(a), cos(a))).gb*0.15 + length(p - 0.5);
        col.rgb    -= length(p - vec2(f0,f1)) * 0.75 /f2;

    gl_FragColor = vec4(col.rgb, color.a );
}




