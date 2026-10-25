precision mediump float;

// Opal Bloom
//
// Repeated rounded shapes stretch through a rippling distortion, opening into petals and closing around small holes. Several offset layers overlap, soften, and separate, giving the surface a blooming, unfurling motion.
//
// par_a.y - increases softness and separates the red, green, and blue shapes.

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

vec2 mouse = par_a.xy;


const float ITERATIONS = 3.;
const float INITIAL_LUMA = .5;

mat2 rotate2d(float _angle){
    return mat2(cos(_angle),-sin(_angle),
                sin(_angle),cos(_angle));
}


float getColorComponent(in vec2 st, in float modScale, in float blur, in float shapeSize) {
    vec2 modSt = mod(st, 1. / modScale) * modScale * 2. - 1.;
    float dist = length(modSt);
    float angle = atan(modSt.x, modSt.y);
    float shapeMap = smoothstep(shapeSize + blur, shapeSize - blur, dist);
    return shapeMap;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 st =
        (2.* fragCoord - tres.xy)
        / min(tres.x, tres.y);
    st *= rotate2d(sin(time * .6 + st.x + st.y) * .3);
    st *= (sin(time * .3) + 2.) * .3;

    st *= log(length(sin(st * 5.18)) * (sin(time) + 2.) * 3.);
    float modScale = 1.;

    vec3 color_out = vec3(0);
    float luma = INITIAL_LUMA;
    float blur = par_a.y; //.2;
    float shapeSize = .2 + (sin(time * .7) + 1.) * .2;
    for (float i = 0.; i < ITERATIONS; i++) {
        vec2 center = st + vec2(sin(time * .5), cos(time * .3));
        //center += pow(length(center), 1.);
        vec3 shapeColor = vec3(
            getColorComponent(center - st * par_a.y, modScale, blur, shapeSize),
            getColorComponent(center, modScale, blur, shapeSize),
            getColorComponent(center + st * par_a.y, modScale, blur, shapeSize)        
        ) * luma;
        st *= 1.1;
        st *= rotate2d(sin(time  * .05) * .3);
        color_out += shapeColor;
        color_out = clamp(color_out, 0., 1.);
        if (color_out == vec3(1)) break;
        luma *= .8;
        blur *= .63;
    }
    fragColor = vec4(color_out,color.a);
}

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}