// name of the Shader                          
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

   float f0 = mix(0.2, 1.8, par_a[0]);    //f0:x pos:
   float f1 = mix(0.5, 2.00, par_a[1]);    //f1:y pos:
   float f2 = mix(0.05, 0.95, par_a[2]);    //f2:bright radius:

vec3 hsv(in float h, in float s, in float v)
{
	return mix(vec3(1.0), clamp((abs(fract(h + vec3(3, 2, 1) / 3.0) * 6.0 - 3.0) - 1.0), 0.0 , 1.0), s) * v;
}

void main( void )
{
	vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / tres.xy;
	p.x *= tres.x / tres.y;
	vec2 c = vec2(-time*0.00154, time*0.2485);
	float d = 1.0;
	vec3 col = vec3(0);
	float t = time;
	for (int i = 0; i < 3; i++) 
		{
		float r = length(p);
		p /= r;
		p = asin(sin(p/r + c));
		col += hsv(r, max(1.60-dot(p,p), 0.0), 1.0);
		}
	gl_FragColor = vec4(sin(col)*0.5+0.5, color.a );
}