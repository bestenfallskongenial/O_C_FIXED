precision mediump float;

// Cubic Islands
//
// A still fractal figure forms a central island surrounded by stepped contour bands. Adjusting its parameters reshapes the boundary into rounded lobes, branching edges, or separated fragments, like an archipelago being folded into a new arrangement.
//
// par_a.x - changes the horizontal component of the cubic fractal constant.
// par_a.y - changes the vertical component of the cubic fractal constant.

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

const int maxIterations = 16;

void main( void )
{
	vec3 colorout; 
	vec2 z;
	vec2 dm;
	vec2 tmp; 
	int j; 
	float a;
	z = gl_FragCoord.xy;
	z.x = (z.x / tres.x) * 2.0 - 1.0;
	z.y = (z.y / tres.y) * 2.0 - 1.0;
	z.x *= 1.8;
	dm.x = (par_a[0]) * 2.0 - 1.0;
	dm.y = (par_a[1]) * 2.0 - 1.0;
	dm.x *= 1.8;
	dm = par_a.xy;
	dm.x = (dm.x * 2.0) - 1.0;
	dm.y = (dm.y * 2.0) - 1.0;
	for (int i = 0; i <= maxIterations; ++i)
		{
		if (tmp.x * tmp.x + tmp.y * tmp.y > 1000.0)
			{ 
			break;
			}
		j = i;
		tmp = z;
		z.x = tmp.x*tmp.x*tmp.x - 3.0*tmp.x*tmp.y*tmp.y + dm.x;
		z.y = 3.0*tmp.x*tmp.x*tmp.y - tmp.y*tmp.y*tmp.y + dm.y;
		}
	if (j == maxIterations)
		{
		colorout = vec3(0.0, 0.0, 0.0);
		}
	else 
		{
		a = 1.0 - (float(j) / 40.0);
		colorout = vec3(a, a, a);
		}
	gl_FragColor = vec4(colorout, color.a);
}