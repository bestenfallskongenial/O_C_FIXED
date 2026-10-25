precision mediump float;

// Folded Ivory
//
// Nested black-and-white contours fold into angular chambers, crooked ribbons, and small triangular pockets. Their slowly shifting arrangement resembles ivory paper cut and folded into an intricate maze.
//
// par_a.y - increases the waviness applied before the repeated folds.
// par_a.w - shifts the folding pattern, changing its arrangement and shapes.

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

float f0 = mix(0.01, 0.095, par_a[0]);
float f1 = mix(0.01, 0.095, par_a[1]);
float f2 = mix(0.0,  1.0,   par_a[2]); 

//change these constants to get different patterns!

float c2 = par_a[3];
#define ITERATION 4
#define c1 vec4(.10+c2, 1.5+c2,.15,.0)

vec2 triangle_wave(vec2 a,float scale)
{
    return abs(fract((a+c1.xy+sin(time*.01))*scale)-.5) ;
}

void main( void )
{
	vec2 uv = (gl_FragCoord.xy-tres.xy)/tres.y/ 2.0;
	// move it continuously upwards
	uv.y += sin(time*.000)*12.;
	// make some twist
	uv.x += sin(uv.y*12.)*f1;
	uv.y += cos(uv.x*12.)*f1;
	float offset = sin(time*.0)*.0 +.2;
	float scale  = 2.16;
	for(int i=0;i<ITERATION;i++)
		{
		uv = triangle_wave(uv.yx-offset,scale)+triangle_wave(uv, scale + float(i)*.08);
		}
	float shade  = smoothstep( .02,.3, ((uv.x)-(uv.y)));

	gl_FragColor = vec4(vec3(shade), color.a );
}