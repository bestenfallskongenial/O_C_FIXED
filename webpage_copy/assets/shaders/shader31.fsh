//                                          
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

float f0 = mix(0.01, 1.39, par_a[0]);   //f0:stretch:
float f1 = mix(0.01, 0.19, par_a[1]);   //f1:beams:
float f2 = mix(0.01, 0.19, par_a[2]);   //f2:zigs:

mat2 roti(float a)
{
    float s=sin(a),c=cos(a);
    return mat2(c,s,-s,c);
}

float shard, ssoft;

float t,as;

vec3 fractal(vec2 p)
{
    p=vec2((atan(p.x,p.y)*3.14),length(p)*.5-1.);
    p+=1.+as*.5;
    p.x=fract(p.x*.1-t*.5+ssoft*.05);
    vec2 m=vec2(100);
    float ml=100.;
    for (int i=0; i<2; i++)
        {
        p=abs(p)/clamp(abs(p.x*p.y),f1+floor(mod(time*.2,5.))/10.,f0)-2.;
        m = min(m, abs(p))+fract(p.x*.5+ssoft*3.+time*.4)+fract(p.y*.5+time*.4);
        ml=min(ml,length(p));;
        }
    m=exp(-2.*m);
    vec3 c=vec3(m.x*10.,length(m*3.),m.y*5.);
    // c=c.rgg;
    ml=exp(-7.*ml)*2.;
    c+=ml;
    return c;
}

void mainImage( out vec4 color_out, in vec2 fragCoord )
{
    vec3 c=vec3(0.);
    float ss=.4+abs(fract(time*.2))*.7;
    as = .5;
    shard = ss*ss*ss;
    ssoft = ss;
    vec2 uv = -1. + 2. * fragCoord.xy/tres.xy;
    t=time*.3;
    float s=sin(time/4.);
    uv+=vec2(sin(t),cos(t))*.3;
    uv*=1.3+tan(ss*4.)*f2;
    //uv.x*=1./pow(abs(s), .6)*sign(s);
    uv.x*=tres.x/tres.y;
    //uv=uv.yx;
 // uv*=.7;
    //t*=.5+floor(length(uv)*6)*.1;
    t+=smoothstep(.0,.2,fract(length(uv*.5)-time*.5));
 // uv*=1.+exp(-3*length(uv))*spectrum.x*100;
    //uv=abs(.5-fract(uv));
    c+=fractal(uv);
    c+=max(0.,1.1-length(uv)*shard*5.);
    //c=mix(c,tex(prevFrame,gl_FragCoord.xy/tres).rgb,.8);
    //c*=step(length(uv),3.);
    //c*=mod(length(uv),.5)*5.;

    color_out = vec4(c, 1.0);//*mod(gl_FragCoord.y,10.)*.1;
}

void main(void)
{
    vec2 uv = gl_FragCoord.xy/tres.xy-.5;
    float ap=tres.x/tres.y;
    ap=1.;
    uv.x*=ap;
    uv*=1.55;
    uv+=vec2(-.07,.17);
    vec3 col=vec3(0.);
    vec4 color_out=vec4(0);
    if ((uv.x<.5*ap&&uv.x>-.5*ap&&uv.y<.5&&uv.y>-.5)) 
        {
        mainImage(color_out,(uv+=.5)*tres);       
        col = color_out.rgb;
        } 
    else 
        {
        uv*=.02;
        mainImage(color_out,(uv+=.5)*tres);       
        col = color_out.rgb;
        }
    gl_FragColor = vec4(col,color.a);
}