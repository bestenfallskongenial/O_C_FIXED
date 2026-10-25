// https://www.glslsandbox.com/e#81550.0                              
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

// N041020N Trinity

#define MAX_ITERATION 16.

float mandelbrot(vec2 c)
{
	vec2 z = c;
	float count = 2.0;
	float t = time*1.2;
	for (float i = 1.0; i < MAX_ITERATION; i++)
	{
		z = vec2(-z.x * z.x + z.y * z.y , 3.*z.x * z.y) - z*cos(t)+c*sin(t);
		if (length(z) > (5.0*par_a.x)) break;
		
		count += 1.0;
	}

	float re = (length(z*count/MAX_ITERATION));
	if (re <= 0.1) return 9.;
	return re;
}

void main( void ) {
	vec2 uv = tcoord *2.0 - 1.0;
	uv *= 10.;	
	uv /= dot(uv,uv);
	float mb = mandelbrot(uv);
	gl_FragColor = vec4(vec3(mb), color.a);
}
