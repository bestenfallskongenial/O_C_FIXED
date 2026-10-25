precision mediump float;

// Three Quiet Ripples
//
// White contour rings gather around three wandering centers. They press into one another, merge into winding channels, then loosen again—like ripples drawn in ink on a black pond.
//
// par_a.x - sets the first center's movement amplitude.
// par_a.y - sets the second center's movement amplitude.
// par_a.z - sets the third center's movement amplitude.
// par_a.w - sharply increases ring brightness and white clipping.

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

void main( void ) 
{
	float t = 1000.0*sin(0.001*time); 
	vec2 uv = 2.0*(gl_FragCoord.xy-0.5*tres.xy)/tres.x;
	vec2 x1 = vec2(-0.4, -0.2) + par_a.x*vec2(sin(t), cos(1.4*t)); 
	vec2 x2 = vec2(0.2,   0.2) + par_a.y*vec2(sin(2.7*t), cos(3.66*t)); 
	vec2 x3 = vec2(0.4,   -0.2)+ par_a.z*vec2(sin(0.3*t), cos(0.4*t)); 
	float f = exp(-10.0*dot(uv-x1, uv-x1))+exp(-5.0*dot(uv-x2,uv-x2))+exp(-20.0*dot(uv-x3,uv-x3));
	f = (1.0+par_a.w)*sin(2.0*3.1415926*mod(f, 0.1)*10.0);
	f = pow(f,20.0);
	gl_FragColor = vec4(vec3(f), color.a); 
}