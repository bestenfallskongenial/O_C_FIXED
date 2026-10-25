precision mediump float;

// Streaked Wells
//
// Two wandering centers pull the surrounding field into tightly nested contours. Directional streaks cross their curved boundaries, producing a moving mesh of arcs and highlights around small dark wells.
//
// par_a.x - shifts the field’s center horizontally.
// par_a.y - shifts the field’s center vertically.
// par_a.z - changes the density and strength of the streaked highlights.

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
 
    //f0:horz center:
//f1:vert center:
//f2:highlights:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

  /* fragment fun by guti */

float K = (1.0/6.0);
float K2 = (16.0) * f2;
float K3 = (0.5/K2);

vec3 rgbFromHue(in float h)
{
h = h - floor(h);
float r = smoothstep( 2.0*K, 1.3*K, h) + smoothstep( 4.0*K, 5.0*K, h);
float g = smoothstep( 0.0*K, 1.0*K, h) - smoothstep( 3.0*K, 4.0*K, h);
float b = smoothstep( 2.0*K, 3.0*K, h) - smoothstep( 5.0*K, 6.0*K, h);
return vec3(r,g,b);
}

void main( void )
{
vec2 rP = 2.0 * ( gl_FragCoord.xy / tres.xy ) + vec2(f0,f1) * -2.;
rP *= vec2(tres.x/tres.y, 1.0);

vec2 P0 = vec2(sin( time*2.0013),cos( time*0.5999))*0.5;
vec2 P1 = vec2(cos( time*1.4011),sin( time*2.0007))*0.5;

float d0 = distance(rP,P0);
float d1 = distance(rP,P1);

vec3 color_out =  vec3( rgbFromHue(2.0/d0 - 2.0/d1 +  time) * smoothstep(10.01, 10.0, 1.0/d0 + 1.0/d1));

float factor = tres.x/K2;

color_out.r *= color_out.r*0.5+mod(gl_FragCoord.y, factor * d0) * K3;
color_out.g *= color_out.g*0.5+mod(gl_FragCoord.x, factor * d1) * K3;
color_out.b *= color_out.b*0.5+mod(gl_FragCoord.y - gl_FragCoord.x, factor * d0-d1) * K3;
gl_FragColor = vec4(color_out, color.a );

}


