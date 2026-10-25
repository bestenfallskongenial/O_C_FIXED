precision mediump float;

// Twisting Triangles
//
// Nested triangular contours twist into sweeping curved bands. Their center drifts while the entire arrangement rotates and breathes, pulling straight-sided layers into curling corners like a stack of flexible triangular sheets.
//
// par_a.x - increases radius-dependent twist.
// par_a.y - increases movement of the triangular pattern’s center.
// par_a.z - increases coordinate scale, making the contour pattern finer.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:twist:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:count:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:zoom:

// "it"  inverse transform
// "pos" position
// "cen" center

vec2 it_stretch(vec2,vec2,float);
vec2 it_rotate(vec2, vec2, float);
vec2 it_polygon_stretch(vec2, vec2, int, float);
vec4 color_by_position_polygon(vec2, vec2, int);

float twoPI;
vec3 t;

void main( void )
{
twoPI = 4.0 * atan(1.0,0.0);

vec2 pos = ( gl_FragCoord.xy - tres.xy*0.5 )/ tres.xx;
vec2 cen = vec2(0);
float r = length(pos) * 2. * f0;
t = 0.5*vec3(time, sin(time), sin(1.2*time));
pos = it_rotate(pos, cen, 0.2*t.x+t.y+10.0*t.z*r);
pos = it_stretch(pos * f2, cen, 2.0+t.z);
cen += 0.3*vec2(t.y,t.z) ;
gl_FragColor = color_by_position_polygon(pos.xy, cen, 3);

}

vec2 it_stretch(vec2 pos, vec2 center, float factor)
{
vec2 d = pos-center ;
return center + d / factor;
}

vec2 it_rotate(vec2 pos, vec2 center, float alpha)
{
vec2 d = pos-center;
return vec2(cos(alpha)*d.x - sin(alpha)*d.y, sin(alpha)*d.x + cos(alpha)*d.y);
}

vec2 it_polygon_stretch(vec2 pos, vec2 center, int N, float factor)
{
vec2 d = pos-center;
float n = float(N);
float angle = atan(d.y, d.x);
float section_angle = ( floor(n * angle / twoPI) + 0.5 ) * twoPI / n;
vec2 section_direction = vec2(cos(section_angle), sin(section_angle));
d = section_direction * dot(section_direction, d) ;
return pos + d * (1.0 - 1.0 / factor );
}

vec4 color_by_position_polygon(vec2 pos, vec2 center, int N)
{
float r;
if(N==0)r = length(pos);
else
{
vec2 d = pos-center  * f1;
float n = float(N);
float angle = atan(d.y, d.x);
float section_angle = ( floor(n * angle / twoPI) + 0.5 ) * twoPI / n;
r = cos(section_angle)*d.x + sin(section_angle)*d.y;
}
float angle = atan(pos.y, pos.x);
r+=0.01*sin(angle+t.x)*cos(angle+t.z)*(t.y + 2.0);
return vec4(fract(vec3(11.0,21.0,37.0)*sqrt(r)+t.x*0.2+t.y*0.5),color.a );
}



