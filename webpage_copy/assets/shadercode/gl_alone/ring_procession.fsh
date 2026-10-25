precision mediump float;

// Ring Procession
//
// Rows of outlined circles travel across a narrow horizontal window. Neighboring rows swell and contract in opposing phases, making the rings overlap, separate, and pass through one another like a procession of interlocking springs.
//
// par_a.x - increases the height of the visible window.
// par_a.y - increases spacing between the circle centers within each repeated group.
// par_a.z - reduces ring radius as it increases.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of tex samplers  uniform int tex_l;     
    
  //f0:reveal:
//f1:density:
//f2:block radius:

float f0 = mix(0.20, 1., par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

// http://glslsandbox.com/e#48993.1

 
// original https://twitter.com/beesandbombs/status/1035545423069503488
// forked https://www.shadertoy.com/view/XlVcW1
#define RAD .1
#define e ( 3. / tres.y )

float tt = .12 *f1;

float makeCircle(in vec2 uv, in vec2 c, in float r)
{
return smoothstep(e, 0., abs( distance(uv, c) - r /f2) );
}

float row(in vec2 uv)
{
    float color_out = 0.,
    s = sign(mod(floor((uv.y * .5 + .5)/RAD), 2.) - .5),
    rad = RAD/2. + RAD/2. * sin(uv.x * 4. -  time * 1.5) * .75 * s;
    for(float i=-1.; i<5.; i++)
    color_out += makeCircle(mod(uv, RAD * 2.), vec2(i * tt, RAD), rad);
    return color_out;
}
void main(void)
{

    vec2 uv = (gl_FragCoord.xy * 2. - tres.xy)/tres.y + vec2( time * .05, 0.);
    gl_FragColor = vec4(row(uv) * step(abs(uv.y), .65 *f0));
   }//endof main


