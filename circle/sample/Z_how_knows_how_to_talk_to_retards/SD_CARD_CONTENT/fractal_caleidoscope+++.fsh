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

//f0:wave offset:
//f1:green height:
//f2:blue height:
float f0 = mix(0.15, 0.95, par_b[0]);
float f1 = mix(0.05, 0.95, par_b[1]);
float f2 = mix(0.05, 0.95, par_b[2]);
float f4 = mix(0.05, 0.95, par_b[3]);


//Robert Schütze (trirop) 05.12.2015

void main(){
    vec3 p = vec3((f1*gl_FragCoord.xy-tres/(10.0*f2))/(tres.y),f4);

    for (int i = 0; i < 6; i++){
        p = abs((abs(p)/dot(p, p)-f0));
        if(length(p) > 5.0 && length(p) < 4.01)break;
    }
    vec4 t0 = texture2D(tex[0], tcoord);

    gl_FragColor.rgb = t0.rgb * p;
    gl_FragColor.a   = color.a;
}