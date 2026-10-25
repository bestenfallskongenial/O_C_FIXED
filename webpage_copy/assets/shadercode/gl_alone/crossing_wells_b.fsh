precision mediump float;

// Concentric Crosscurrents
//
// Broad arcs pass through one another around wandering centers. Their contours tighten into dense rings near each center, then spread into smoother bands, like several circular currents continually reshaping the same surface.
//
// par_a.x - compresses the pattern horizontally and increases red-channel variation.
// par_a.y - compresses the pattern vertically and raises the green-channel level.
// par_a.z - raises the red-channel offset while reducing blue-channel intensity.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:horz mod:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:vert mod:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:mod red+blue:

float makePoint(float x,float y,float fx,float fy,float sx,float sy,float t)
{
   float xx=x+cos(t*fx*2.1)*sx;
   float yy=y+tan(t*fy*0.1)*sy;
   return 1.0/sqrt(xx*xx+yy*yy);
}

void main( void )
{
   vec2 p=(gl_FragCoord.xy/tres.y)/1.0-vec2(1,sin(tres.y*tres.y)/20.0);
   //p=p*1.0;
   float x=p.x * f0;
   float y=p.y * f1;
   float a=    makePoint(x,y,0.5,0.5,0.5,0.5,time);
         a=a+  makePoint(x,y,1.9,2.0,0.4,0.4,time);
         a=a-  makePoint(x,y,0.8,0.7/time,0.4,0.5,time); // ?????
         a=a+  makePoint(x,y,2.3,0.1,0.6,0.3,time);
         a=a-  makePoint(x,y,0.8,1.7,0.5,0.4,time);

   float b=    makePoint(x,y,1.2,1.9,0.1,0.2,time);
         b=b+  makePoint(x,y,0.7,2.7,0.4,0.4,time);
         b=b+  makePoint(x,y,1.4,0.6,0.4,0.5,time);
         b=b+  makePoint(x,y,2.6,0.4,0.6,0.3,time);
         b=b+  makePoint(x,y,0.7,1.4,0.5,0.4,time);

   float c=    makePoint(x,y,3.7,0.3,0.3,0.3,time);
         c=c+  makePoint(x,y,1.9,1.3,0.4,0.4,time);
         c=c+  makePoint(x,y,0.8,0.9,0.4,0.5,time);
         c=c+  makePoint(x,y,1.2,1.7,0.6,0.3,time);
         c=c+  makePoint(x,y,0.3,0.6,0.5,0.4,time);
   vec3 d = vec3(cos(a)*f0+(.5*f2),sin(b)+f1,sin(c)/f2)/sin(2.0 );

   gl_FragColor = vec4(d.x,d.y,d.z,color.a);
}