// www.shadertoy.com/view/XdjSRK MORE                                       
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

float f0 = mix(0.05, 0.95, par_a[0]);  //f0:branches:
float f1 = mix(0.05, 0.95, par_a[1]);  //f1:scale:
float f2 = mix(0.05, 0.95, par_a[2]);  //f2:img offset:

const float TWO_PI = 3.141592*2.0;

//ADJUSTABLE PARAMETERS:
float branches = floor(24.0  * f0);
float scale = 0.8* f1 + 0.125; // try different values :)
float speed = 0.5 ;  // 10.0 ;

//Complex Math:
vec2 complexExp(in vec2 z)
{
return vec2(exp(z.x)*cos(z.y),exp(z.x)*sin(z.y));
}

vec2 complexLog(in vec2 z)
{
return vec2(log(length(z)), atan(z.y, z.x));
}

vec2 complexMult(in vec2 a,in vec2 b)
{
return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

float complexMag(in vec2 z)
{
return float(pow(length(z), 2.0));
}

vec2 complexReciprocal(in vec2 z)
{
return vec2(z.x / complexMag(z), -z.y / complexMag(z));
}

vec2 complexDiv(in vec2 a,in vec2 b)
{
return complexMult(a, complexReciprocal(b));
}

vec2 complexPower(in vec2 a, in vec2 b)
{
return complexExp( complexMult(b,complexLog(a))  );
}

//Misc Functions:
float nearestPower(in float a, in float base)
{
return pow(base,  ceil(  log(abs(a))/log(base)  )-1.0 );
}
float map(float value, float istart, float istop, float ostart, float ostop)
{
   return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

vec4 droste(in vec2 co)
{

//SHIFT AND SCALE COORDINATES TO <-1,1>
vec2 z = (co-0.5+sin( time*0.3)*0.35)*2.0;

//ESCHER GRID TRANSFORM:
float factor = pow(1.0/scale,branches);
z = complexPower(z, complexDiv(vec2( log(factor) ,TWO_PI), vec2(0.0,TWO_PI) ) ); 
//RECTANGULAR DROSTE EFFECT:
z *= 1.0+fract( time*speed)*(scale-1.0);
float npower = max(nearestPower(z.x,scale),nearestPower(z.y,scale));
z.x = map(z.x,-npower,npower,-1.0,.40);
z.y = map(z.y,-npower,npower,-1.0,.20);

//UNDO SHIFT AND SCALE:
z = z*0.5+f2;

return texture2D(tex[0],z);

}

void main(void)
{

vec2 uv = gl_FragCoord.xy / tres.xy;
vec4 result = droste(uv);

gl_FragColor = vec4(result.rgb, color.a);

}//endof main




