precision mediump float;

// Six Wandering Lights
//
// Six luminous centers follow different sweeping paths, their soft halos spreading across one another. Bright knots merge and separate as the lights move through and beyond the frame, leaving broad overlapping fields like lanterns passing behind translucent glass.
//
// par_a.x - increases the red-channel contribution.
// par_a.y - increases the green-channel contribution.
// par_a.z - increases the blue-channel contribution.
// par_a.w - increases brightness and the apparent size of the glows.

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

#define PI 3.14159265358979
#define N 6
vec3 circle(vec2 pos,float size,vec3 color_out){
	return color_out * size / distance(pos,tcoord);
}

void main(void){
	float t = time * 0.08;
	float theta = 11.0;
	float r = 0.6;
	vec2 pos = vec2(0.0);
	vec3 color_out = vec3(0.0);
	for(int i=0; i<N; i++){
		float size = float(i) * 0.005+par_a.w;
		theta += PI / (float(N)*0.5);
		pos = vec2(cos(theta*t)*r,sin(theta-t)*r);
		vec3 c = vec3(0.0);
		c.r = par_a.x * cos(t*float(i));
		c.g = par_a.y * sin(t*float(i));
		c.b = par_a.z * sin(float(i));
		color_out += circle(pos,size,c);
	}
	gl_FragColor = vec4(color_out,color.a);
}