// www.glslsandbox.com/e#102935.0                    
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

// Emulate a black texture
// #define texture(s, uv) vec4(0.0)

//uniform sampler1D texFFT; // towards 0.0 is bass / lower freq, towards 1.0 is higher / treble freq

//  #define B (texture(iChannel0,vec2(.1,.75)))			// org
//  #define BB (texture(iChannel0,vec2(.1,.75)))		// org

//  #define B (vec4(0.5)) 								// some fixed middle value
//  #define BB (vec4(0.5))

	#define B (vec4(0.5 + 0.5 * sin(time))) 			// oscillating value between 0-1
	#define BB (vec4(0.5 + 0.5 * sin(time * 1.1))) 		// slightly different frequency

//uniform sampler1D texFFTSmoothed; // this one has longer falloff and less harsh transients
//uniform sampler1D texFFTIntegrated; // this is continually increasing

#define F float
#define V vec2
#define W vec3
#define N normalize
#define L length
#define rot(x) mat2(cos(x),-sin(x),sin(x),cos(x))
#define S(x) sin(x+2.*sin(x))
#define col(x) (cos((x+W(0,.3,.4))*6.28)*.5+.5)

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = (fragCoord-.5*tres.xy)/tres.y;

	F i=0.,d=0.,e=1.;
	W p,pI, rd=N(W(0,0,1));
	rd.zy*=rot(uv.y*2.);
	rd.xz*=rot(-uv.x*2.5+S(time*.1)*4.+.03*S(time+uv.x*2.));
	F c;
	for(F ii=1.;ii<=3.;ii++){
		if (e<=.0001) break;
		pI=p=d*rd;
		F sz=.25*BB.x;
		sz = max(sz,.1);
		p.z+=(time*.5)+B.x*.01;
		p.zy=p.yz;
		F s,ss=1.5;
        p.xz*=s=1.+.5*S(pI.y*2.-time);
        ss*=s;

		//p.xz*=rot(S(time*.4));
		 c=0.;
		for(F j=1.;j<=1.;j++){
		p.xz*=rot(time+S(time*.4*1.61+pI.z*1.+j));
			ss*=s=3.;
			p*=s;
			p.y+=.5+j/10.;//+B.x;
			p.y=fract(p.y)-.5;
			p=abs(p)-.5-B.x*.1 + .2*S(pI.z*.1+time*.1);
			if(p.z<p.x)p.xz=p.zx;
			if(p.y>p.x)p.xy=p.yx;
			c+=L(p)*.01;
		}
		
		p-=clamp(p,-sz,sz);
		d+=e=(L(p.xz)-.0001)/ss;
		i++;
	}
	fragColor.rgb = 20./i*col(log(d)*.8+c*20.+time*.1);
    fragColor.a=color.a;
}
// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
}