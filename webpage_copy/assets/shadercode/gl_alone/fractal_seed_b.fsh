precision mediump float;

// Inverted Blossom
//
// An inverted fractal field gathers into a central figure with nested contour boundaries. Its edges change through a repeating cycle, opening small branches and closing them into smoother regions, like a blossom folding back toward its seed.
//
// par_a.x - changes the horizontal component of the fractal constant.
// par_a.y - changes the vertical component of the fractal constant.

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


#define thresh 4.0
#define stop 6

vec3 rgb(float c, float f,float o)
{
	return vec3(sin(o+c),sin(o+c+f),sin(o+c+2.*f))*.5+.5;
}

vec2 cmul(vec2 a, vec2 b)
{
	return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 cdiv(vec2 v1, vec2 v2)
{
	float a = (v1.x*v2.x+v1.y*v2.y)/(v2.x*v2.x+v2.y*v2.y);
	float b = (v1.y*v2.x-v1.x*v2.y)/(v2.x*v2.x+v2.y*v2.y);
	return vec2(a,b);
}

vec3 jf(vec2 z)
{
	z = cdiv(vec2(.1),z);

    vec2 c = (2. * par_a.xy - 1.) * 2.;

	int _i;
	for(int i = 0; i < stop; i++)
	{
		z = cmul(z, c - z) + cdiv(c, 8.2 * z * vec2(mod(time, 10.0)));
		_i = i;
		if (length(z) > thresh) break;
	}
	vec3 col = rgb(log(0.9 + float(_i)/12.)*2.3, 1.0, 9.0);
	return col;
}

void main(void)
{
    vec2 z = (2.0 * gl_FragCoord.xy - tres.xy) / tres.y;
    vec3 col = vec3(0.0);
    float a = 0.002;
    float b = 0.3333;

    col += jf(z + vec2(1.0, 0.0) * a) * b;
    col += jf(z + vec2(0.0, 0.0) * a) * b;
    col += jf(z + vec2(0.0, 1.0) * a) * b;

    gl_FragColor = vec4(col, color.a);
}
