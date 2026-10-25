precision mediump float;

// Converging Filaments
//
// Narrow luminous contours gather around a circular transition and stretch into sweeping branches. Their intersections brighten and separate as the underlying phases turn, like filaments being drawn through a shifting lens.
//
// par_a.x - changes the outer radial transition and the reach of the animated displacement.
// par_a.y - changes the inner radial transition.

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

void main(void){
	
	vec2 p = (gl_FragCoord.xy * 2.0 - tres) / min(tres.x, tres.y);
	vec3 color_out = vec3(0.0, 0.3, 0.5);
	float d = 0.8-smoothstep(mouse.y,mouse.x,length(p));
	float f = 0.0;
	float PI = 3.141592;
	for(float i = 0.0; i < 16.0; i++){
		
		float s = sin(0.5*time + i * PI / 40.0) * mouse.x;
		float c = cos(0.5*time + i * PI / 40.0) * 10.5;
 
		f +=  0.01 / ( d + p.x + c * d + p.y + s);
	}
	
	
	gl_FragColor = vec4(vec3(f * color_out), color.a);
}