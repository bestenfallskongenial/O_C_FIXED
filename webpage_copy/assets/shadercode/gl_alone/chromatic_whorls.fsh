precision mediump float;

// Chromatic Whorls
//
// Three layers of nested bands travel around shifting focal regions. Their contours tighten into small rings, open into broad curved sheets, and slide across one another, like neighboring whorls pulling a flexible surface between them.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:accent lines:
float f1 = mix(0.2, 0.8, par_a[1]);     //f1:spatial pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:squeeze:

vec2 mouse = par_a.xy;

#define pi2_inv 0.0

float border(vec2 uv, float thickness){
	uv = fract(uv - vec2(0.5));
	uv = min(uv, vec2(1.)-uv)*2.;
//	return 1./length(uv-0.5)-thickness;
	return clamp(max(uv.x,uv.x)-1.+thickness,0.,1.)/thickness;;
}

vec2 div(vec2 numerator, vec2 denominator){
   return vec2( numerator.x*denominator.x + numerator.y*denominator.y,
                numerator.y*denominator.x - numerator.x*denominator.y)/
          vec2(denominator.x*denominator.x + denominator.y*denominator.y);
}

vec2 spiralzoom(vec2 domain, vec2 center, float n, float spiral_factor, float zoom_factor, vec2 pos){
	vec2 uv = domain - center;
	float d = length(uv);
	return vec2( atan(uv.y, uv.x)*n*pi2_inv + log(d)*spiral_factor, -log(d)-zoom_factor) + pos;
}

void main( void ) {
	vec2 uv = gl_FragCoord.xy / tres.xy;
	uv = 0.5 + (uv - 0.5)*vec2(tres.x/tres.y,1.);
	
	
	vec2 p1 = vec2(0.2,0.5);
	vec2 p2 = vec2(0.8, 0.5);

	vec2 moebius = div(uv-p1, uv-p2);
	moebius.x += sin(time);
	moebius.y += cos(time);
	uv = uv-0.5;
	

	vec2 spiral_uv = spiralzoom(moebius,vec2(0.),8.,.90,56.67,vec2(555.123,123.5)-time*1.1);
	vec2 spiral_uv2 = spiralzoom(moebius,vec2(0.),3.,.9,1.2,vec2(+0.5,0.5)*time*.3);
	vec2 spiral_uv3 = spiralzoom(moebius,vec2(0.),5.,.75,4.0,-vec2(0.5,0.5)*time*.4);

	gl_FragColor = vec4(border(spiral_uv,0.9), border(spiral_uv2,0.9) ,border(spiral_uv3,0.9),color.a);

	vec2 weed_uv = (uv);
	weed_uv.y += 0.38;
	float w = 1.0;
	float r = .23;
	float o = atan(weed_uv.y, weed_uv.x);
	r *= (1.+sin(o))*(1.+0.9 * cos(8.*o))*(1.+0.1*cos(24.*o))*(0.9+0.05*cos(200.*o));
	float l = length(weed_uv);

	float d = clamp(1.-abs(l - r + w*2.)/w, 0., 1.);

	gl_FragColor.rgb /= 1.25-d;
}