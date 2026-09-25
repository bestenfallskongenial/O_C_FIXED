// Random Section Color Separation Shader               
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

// Pseudo-random function
float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}
// Hue shift function
vec3 hueShift(vec3 color, float hueShift)
{
    const vec3 k = vec3(0.57735, 0.57735, 0.57735);
    float cosAngle = cos(hueShift);
    return vec3(color * cosAngle + cross(k, color) * sin(hueShift) + k * dot(k, color) * (1.0 - cosAngle));
}

void main()
{
    vec2 uv = gl_FragCoord.xy / tres;
    // Generate random section sizes
    float sectionSizeX = mix(0.1, 0.5, random(vec2(time, par_a[0]))); //random(vec2(time, 0.1)
    float sectionSizeY = mix(0.1, 0.5, random(vec2(time, par_a[0]))); //random(vec2(time, 0.2)
    // Calculate section indices
    int sectionX = int(uv.x / sectionSizeX);
    int sectionY = int(uv.y / sectionSizeY);
    // Generate a random value for this section
    float sectionRandom = random(vec2(float(sectionX), float(sectionY)));
    // Sample the original tex
    vec4 texColor = texture2D(tex[0], uv);
    // Apply color separation or hue shift based on the random value
    vec3 finalColor;
    if (sectionRandom < 0.5) 
        {
        // Color separation
        float offsetX = mix(-0.05, 0.05, random(vec2(sectionRandom, par_a[1]))); // random(vec2(sectionRandom, 0.1))
        float offsetY = mix(-0.05, 0.05, random(vec2(sectionRandom, par_a[1]))); // random(vec2(sectionRandom, 0.2))
        float r = texture2D(tex[0], uv + vec2(offsetX, 0.0)).r;
        float g = texColor.g;
        float b = texture2D(tex[0], uv + vec2(0.0, offsetY)).b;
        finalColor = vec3(r, g, b);
        }
    else 
        {
        // Hue shift
        float hueAmount = mix(-3.14159, 3.14159, random(vec2(sectionRandom, par_a[2])));
        finalColor = hueShift(texColor.rgb, hueAmount);
        }
    // Mix the effect with the original color
    float mixAmount = par_a[3] * 2.0;
    finalColor = mix(texColor.rgb, finalColor, mixAmount);
    
    gl_FragColor = vec4(finalColor, texColor.a * color.a);
}