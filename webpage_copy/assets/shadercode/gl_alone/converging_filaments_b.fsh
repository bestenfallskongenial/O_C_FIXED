precision mediump float;

// Filament Constellation
//
// Thin glowing filaments converge into bright knots, fan outward, and reconnect along a horizontal chain. Their curves turn and stretch into delicate loops, like a constellation continually redrawing the threads between its stars.
//
// par_a.y - increases coordinate scale, revealing a wider portion of the filament field.

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
	
	vec2 p = 2.*mouse.y*(gl_FragCoord.xy * 2.0 - tres) / min(tres.x, tres.y);

	vec3 color_out = vec3(0.0, 0.3, 0.5);

	float d = 0.8-smoothstep(0.0,.8,length(p));
	float f = 0.0;
	float PI = 3.141592;
	for(float i = 0.0; i < 8.0; i++){
		
		float s = sin(0.5*time + i * PI / 8.0) * 10.01;
		float c = cos(0.5*time + i * PI / 8.0) * 12.5;
 
		f +=  .01/abs( floor(-d+fract(2.5*d+f)) +p.x * dot(c, -d) *cos(time+p.x) +p.y * s);
	}
	
	
	gl_FragColor = vec4((f + .5*color_out), color.a);
}

/*
// subversion: 
// http://glslsandbox.com/e#60770.1
// http://glslsandbox.com/e#60768.5
*/