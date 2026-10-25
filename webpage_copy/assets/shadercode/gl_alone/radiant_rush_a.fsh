precision mediump float;

// Radiant Rush
//
// Dense streaks stream outward from a dark central opening. Their lengths flicker independently while the whole field slowly turns, creating the impression of rushing through a rotating shower of passing lights.
//
// par_a.x - changes the angular distribution and timing of the streaks.
// par_a.y - increases radial repetition, making the streak pattern denser.
// par_a.z - reduces brightness, making the trails appear thinner.

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

float f0 = mix(0.05, 0.95, par_a[0]);  //f0:order:
float f1 = mix(0.05, 0.95, par_a[1]);  //f1:speed:
float f2 = mix(0.05, 0.95, par_a[2]);  //f2:length:

#define PI 3.1415926535897932384626

float random(float n)
{
   return fract(abs(sin(n * 55.753) * 367.34));
}

mat2 rotate2d(float angle)
{
   return mat2(cos(angle), -sin(angle), sin(angle), cos(angle));
}

void main()
{
   vec2 uv = (gl_FragCoord.xy * 2.0 - tres.xy) / tres.x;

   uv *= rotate2d(time * 0.2);

   float direction = 1.0;
   float speed = time * direction * 1.6;
   float distanceFromCenter = length(uv);

   float meteorAngle = atan(uv.y, uv.x) * (180.0 / PI);

   float flooredAngle = floor(meteorAngle);
   float randomAngle = pow(random(flooredAngle), f0);
   float t = speed + randomAngle;

   float lightsCountOffset = 0.8 * f1;
   float adist = randomAngle / distanceFromCenter * lightsCountOffset;
   float dist = t + adist;
   float meteorDirection = (direction < 0.0) ? -1.0 : 0.0;
   dist = abs(fract(dist) + meteorDirection);

   float lightLength = 100.0;
   float meteor = (5.0 / dist) * cos(sin(speed)) / lightLength;
   meteor *= distanceFromCenter * 2.0 / f2;

   vec3 color_out = vec3(meteor);

   gl_FragColor = vec4(color_out, color.a );
}