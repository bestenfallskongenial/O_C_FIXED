// www.glslsandbox.com/e#106087.0                      
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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:accent lines:
float f1 = mix(0.2, 0.8, par_a[1]);     //f1:spatial pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:squeeze:

//https://iquilezles.org/articles/palettes/
vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos( 6.28318*(c*t+d) );
}
void main( void ) {

	vec2 p = ( gl_FragCoord.xy *2.0 - tres.xy ) /tres.y;
	vec2 p2=p;
	vec3 cc=vec3(0.0);
	for(float i=0.0;i<2.1;i++){
	p=fract(p*1.5);
	p-=0.5;
	float d = length(p)*exp(-length(p2));
	vec3 c=palette(length(p2)+i*0.4+time*0.4);
	//d-=0.5;
	d=sin(d*32.+time)/16.;
	d=abs(d);
	//d=step(0.1,d);
//	d=0.01/d;
	d=pow(0.01/d,2.0);
	c *=d;
	cc+=c;
	}
	cc = 1. - exp( .333/-cc );
	gl_FragColor = vec4( cc, color.a );

}