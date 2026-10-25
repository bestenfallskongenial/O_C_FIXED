precision mediump float;

// Unlit Expanse
//
// An unbroken black field holds the screen in quiet darkness.
//
// par_a.x - adjusts the vertical component of the fractal iteration.
// par_a.y - adjusts the horizontal component of the fractal iteration.

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

#define PI 3.141592653
#define AA 2.

vec3 colMap(float v) {
 v=mod(v, PI+1.3)-.8;
 return vec3(
  sin(sin(v-.6)),
  sin(sin(v)),
  sin(sin(v+.8))
  );
}

void main() 
{
//  const float aaFract = 1./AA;
  for(float aa=0.; aa<1.; aa+=2.0)
  {
      for(float bb=0.; bb<1.; bb+=2.0){
          vec2 uv =  ( (gl_FragCoord.yx + vec2(aa,bb) ) -.5* tres.yx ) / tres.x;
          uv*=(-cos(time*.1)+1.3)*.28;
          uv+=vec2(.1,.65);

          vec2 c =uv;
          vec2 z=c;
          float l=0.;
          float sum=length(z);
          vec2 newZ;
          for(int i=0;i<14;i++){
           c+=(par_a.yx/tres.yx  -.5)*-.01*float(i);
           c+=(par_a.yx  -.5)*-.01*float(i);
            newZ=vec2(-z.y*z.y+z.x*z.x, 2.*z.x*z.y)+c;
            sum+=length(newZ-z);
            z=newZ;
            l=length(z);
            if(l>2.) break;
          }
          vec2 dir=z-c;
          vec3 col = vec3(dir, .0);

          uv.x=mod((atan(dir.y,dir.x)/PI*.5+.5)*6.+(time+sin(time)*.9)*4., 1.);
          uv.y=mod(length(dir*.5),1.);

          col=colMap(sum*.2-l*.1+time*.1).gbr;

          gl_FragColor += vec4(col.grb, color.a); // * aaFract*aaFract;
        }
    }
}
