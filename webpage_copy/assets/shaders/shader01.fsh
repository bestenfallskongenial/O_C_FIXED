// MISTY LIGHTS by WEYLAND YUTANI                
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

uniform sampler2D tex[8];               // Array of texture samplers       
uniform int tex_l;                      // number of loaded texures             

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:control 1:
float f1 = mix(0.3, 0.95, par_a[1]);    //f1:control 2:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:control 3:

// MISTY LIGHTS by WEYLAND YUTANI

float size=f0;

void main(void) {

    vec2 move1,move2,move3,move4;

    move1.x = cos(time*0.5)*size;
    move1.y = sin(time*0.2)*size;

   size += f1 / 4.;

    move2.x = cos(time*0.3)*size;
    move2.y = sin(time*0.4)*size;
  size -= f0;

    move3.x = sin(time*0.7)*size;
    move3.y = cos(time*0.9)*size;

    move4.x = sin(time*0.8)*size;
    move4.y = cos(time*0.1)*size;

    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / tres.xy;
    float r1 =(dot(p-move1,p-move1))*18.0 -f0;
    float r2 =(dot(p+move2,p+move2))*12.0 *f1 + f1;
    float r3 =(dot(p+move3,p+move3))*14.0 * f1;
    float r4 =(dot(p+move4,p+move4))*16.0 -f0 * f2;
    float metaball =(1.0/r1+1.0/r2-1.0/r3-1.0/r4);
    gl_FragColor = vec4(  pow(metaball,f0+move1.x),
                          pow(metaball * f2,1.0),
                          pow(metaball - .2 * f1,1.0), color.a);

    //-mix(vec3(0.2+cos(time/2.5)/5.0, 0.2+sin(time/3.5)/5.0, 0.2+sin(time/3.0)/5.0),vec3(0.0), p.y),1.0); //?
  }


