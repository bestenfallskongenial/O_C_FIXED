precision mediump float;

// Veiled Picture Rosette
//
// The source image opens into mirrored petals around the center. Changing horizontal strips shade parts of the rosette with curling wave patterns, like thin veils passing across a nearly still ornament.
//
// par_a.x - number of mirrored sectors, from 1 to 16; also affects the slow shading modulation.
// par_a.z - reduces one group of shading waves; at maximum, adds a second shaded strip.
// par_a.w - reduces another group of shading waves.

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
const float PI = 3.14159265359;

// Parameter bounds
 float pot0Max = 2.0;

float adc2Min = 0.3;
float adc2Max = 2.0;

float adc3Min = 0.3;
float adc3Max = 2.0;

float adc4Min = 1.0;
float adc4Max = 16.0;

float adc5Min = 1.0;
float adc5Max = 16.0;
  
int numBarsMin = 1;
int numBarsMax = 2;

float barLifetimeMin = 5.0;
float barLifetimeMax = 15.0;

// Function to rotate a 2D vector
vec2 rotate2D(vec2 v, float angle)
{
    float s = sin(angle);
    float c = cos(angle);
    return vec2(v.x * c - v.y * s, v.x * s + v.y * c);
}

// Simplified wave function
float wave(float x, float y, float f2, float f3)
{
    return sin(10.0*x+10.0*y) / 5.0 + sin(20.0*x+15.0*y) / f3 + sin(4.0*x+10.0*y) / -f2 + sin(y) / 2.0 + sin(x*x*y*20.0) + sin(x * 20.0 + 4.0) / f3 + sin(y * 30.0) / 5.0 + sin(x) / f2;
}

// Improved pseudo-random number generator
float random(vec2 st, float seed)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)) + seed) * 43758.5453123);
}

void main()
{
    // Parameter mapping
    float f0 = mix(0.05, 0.95, par_a[0]);
    float f2 = mix(0.05, 0.95, par_a[2]);
    float f3 = mix(0.05, 0.95, par_a[3]);
    float numFolds = floor(adc4Min + par_a[0] * (adc4Max - adc4Min));
    float textureThreshold = adc5Min + par_a[1] * (adc5Max - adc5Min);
    int numBars = int(float(numBarsMin) + par_a[2] * float(numBarsMax - numBarsMin));
    float barLifetime = barLifetimeMin + par_a[2] * (barLifetimeMax - barLifetimeMin);

    // Kaleidoscope effect
    vec2 uv = tcoord * 2.0 - 1.0;
    float angle = atan(uv.y, uv.x);
    float radius = length(uv);
    angle = mod(angle, 2.0 * PI / numFolds);
    angle = abs(angle - PI / numFolds);
    uv = vec2(cos(angle), sin(angle)) * radius;
    uv = rotate2D(uv, time * 0.00001);
    uv = uv * 0.5 + 0.5;

    // Choose tex based on threshold
    vec4 kaleidoscopeColor;
    if (par_a[1] > textureThreshold)
{
        kaleidoscopeColor = texture2D(tex[1], uv);
    }
else
{
        kaleidoscopeColor = texture2D(tex[0], uv);
    }

    vec4 finalColor = kaleidoscopeColor;

    // Apply wave effect to bars after kaleidoscope folding
    for (int i = 0; i < 4; i++)
{ // Max 5 bars
        if (i >= numBars) break;
       float randomSeed = float(i) / 5.0 + time * 0.01; // Add time for continuous randomization
        float barStart = random(vec2(randomSeed, 0.1), time);
        float barEnd = barStart + random(vec2(randomSeed, 0.2), time + 1000.0) * 0.2; // Max 20% of screen height
        float barProgress = mod(time, barLifetime) / barLifetime;
       if (tcoord.y >= barStart && tcoord.y <= barEnd)
{
            float z = wave(tcoord.x, tcoord.y, f2, f3) + 2.0;
            z *= 4.0 * sin(1.57 + (f0 * time * 0.00001) / 5.0);
            float d = fract(z);
            if (mod(z, 2.0) > 1.0)
{
                d = 1.0 - d;
            }
            vec4 waveColor = vec4(finalColor.rgb * d, color.a);

            finalColor = mix(finalColor, waveColor, 1.0);
        }
    }

    gl_FragColor = finalColor;
}