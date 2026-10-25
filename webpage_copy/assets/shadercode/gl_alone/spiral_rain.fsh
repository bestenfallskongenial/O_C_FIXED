precision mediump float;

// Spiral Rain
//
// Streams of elongated drops curve around a dark center. Each trail moves at its own pace while the whole field slowly turns, drawing the falling streaks into spiraling fans and tightly gathered loops.

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

#define PI 3.14159
#define	TAU 6.28318
 
mat2 rot(float a)
{
    float s=sin(a),c=cos(a);
    return mat2(c,s,-s,c);
}
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	
	vec2 uv = (fragCoord.xy - .5 * tres.xy) / tres.y;
	uv *= 8.0;
	float dd1 = length(uv);
	dd1 = smoothstep(0.0,1.5,dd1);
	uv.xy /= dot(uv,uv)*2.25;
	
	float dd = length(uv*.25);
    uv *= rot(dd+fract(time*0.0025)*TAU);
    vec3 col = vec3(.9,2.9,.425)*2.0;	     	// Drop Colour
    uv.x = uv.x*64.0;						// H-Count
    float dx = fract(uv.x);
    uv.x = floor(uv.x);
    float t =  time*0.3;
    uv.y *= 0.15;							// stretch
    float o=sin(uv.x*215.8);				// offset
    float s=cos(uv.x*33.1)*.3 +.7;			// speed
    float trail = mix(95.,35.0,s);			// trail length
    float yv = fract(uv.y + t*s + o) * trail;
    yv = 1.0/yv;
    yv = smoothstep(0.,9.58,yv*yv);
    yv = sin(yv*PI)*(s*5.0);
    float d2 = sin(dx*PI);
    yv *= d2*d2;
    col = col*yv;
    
	col = pow(col,vec3(0.25));
    	
	col *= dd1;
    fragColor = vec4(col,color.a);
}
 
 
void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}//