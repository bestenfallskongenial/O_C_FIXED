precision mediump float;

// Scanned Threads
//
// Four wobbling vertical traces interrupt a dense field of horizontal scanlines. Their paths bend and drift together beneath a slow overall rotation, producing the appearance of embossed threads glimpsed through a finely slotted screen.
//
// par_a.x - increases variation along the wobbling traces.
// par_a.y - increases wobble amplitude.

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

//float count = mix(1.0, 16.0, par_a[2]);
#define count 4.0
//generic rotation formula
vec2 rot(vec2 uv,float a)
{
	return vec2(uv.x*cos(a)-uv.y*sin(a),uv.y*cos(a)+uv.x*sin(a));
}
// generic rand return
float rand(float n)
{
 	return fract(cos(n*89.42)*343.42);
}

void main( void )
{
 if (mod(gl_FragCoord.y, 5.0) > 4.0) // 5 & 4 are the scanned lines 
 	{
	vec2 uv = 2.*( gl_FragCoord.xy / tres.xy )-1. ;
	uv.x *= tres.x/tres.y;
	uv = rot(uv,time*.01);
	float spread = .015;
	float thickness = .04;
	float wobble_freq = .15*(par_a[0]);
	float wobble_ampl = .15*(par_a[1]);
	float t = time*.4;
	vec3 col = vec3(0.0);
	float s = 0.;
	for(float i=0.;i<count;i++)
		{
		// individual wobble here
		uv.x += sin(t * uv.y * wobble_freq*.01 + t * i*.21 + i*.3 ) * wobble_ampl;
		// todo > eliminate ugly unision phase when sin becomes max ?!?
		// make them all move slightly
		uv.x += cos(t + i*uv.y * wobble_freq*.81)*.02;
		// offset each line
		uv.x +=	i*spread - spread*count*.4;
		s += abs( .01 / uv.x* (thickness+i*.02+.02));
		}
	// emboss
	s = smoothstep(.7,.2,s)-.1;
	// lowkey dithering :)
	s -= rand(uv.x)*.06;
	float vig = uv.x*uv.y * 15.0; 
	//vig = pow(vig, 0.25);
	s+=vig*.01-.2;

	gl_FragColor = vec4( vec3(s), color.a );
    }
else 
	{
    gl_FragColor = vec4(0.0, 0.0, 0.0, color.a );
    }
}