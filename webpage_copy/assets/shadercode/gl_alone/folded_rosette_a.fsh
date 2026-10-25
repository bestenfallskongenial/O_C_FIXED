precision mediump float;

// Folded Rosette
//
// A still fractal lattice gathers into nested rings, mirrored pockets, and folded chambers. Turning its controls reshapes the entire arrangement, opening fine rosettes or stretching their structure into broader, softer forms.
//
// par_a.x - changes the folding offset and fractal depth coordinate.
// par_a.y - increases coordinate scale, showing more, smaller structures.
// par_a.z - shifts the pattern’s origin along both axes.

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

//f0:wave offset:
//f1:green height:
//f2:blue height:
float f0 = mix(0.15, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

vec2 mouse = par_a.xy;

//Robert Schütze (trirop) 05.12.2015

void main(){
	vec3 p = vec3((f1*gl_FragCoord.xy-tres/(10.0*f2))/(tres.y),mouse.x);
//	vec2 p2 = vec2(gl_FragCoord.xy/6.5*tres);
	
	for (int i = 0; i < 6; i++){
	   p = abs((abs(p)/dot(p, p)-f0));
	   if(length(p) > 5.0 && length(p) < 4.01)break;
	}
	gl_FragColor.rgb = p;
	gl_FragColor.a = color.a;
}