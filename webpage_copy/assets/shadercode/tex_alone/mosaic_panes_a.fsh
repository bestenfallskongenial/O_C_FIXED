precision mediump float;

// Mosaic Panes
//
// Rectangular panes continually resize across a collection of overlapping source images. Different panes reveal different mixtures, breaking the aligned pictures into a shifting mosaic whose borders can fade from dark frames back into the images.
//
// par_a.x - changes the range of randomly varying pane sizes.
// par_a.y - increases source-image blend strength.
// par_a.z - increases frame width.
// par_a.w - increases image visibility within the frames, reducing their darkness.

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

void main()
{
    vec2 uv = gl_FragCoord.xy / tres;

    // Larger, more consistent section sizes for less frequent changes
    float sectionSizeX = mix(0.2, par_a.x, random(vec2(time, 0.1))); 
    float sectionSizeY = mix(0.2, par_a.x, random(vec2(time, 0.2))); 

    // Calculate section indices
    int sectionX = int(uv.x / sectionSizeX);
    int sectionY = int(uv.y / sectionSizeY);
        // Calculate local coordinates within section
    vec2 localUV = fract(vec2(uv.x / sectionSizeX, uv.y / sectionSizeY));
        // Calculate frame width based on par_a[2]
    float frameWidth = par_a.z * 0.1;
        // Check if we're in the frame area
    bool inFrame =  localUV.x < frameWidth || localUV.x > (1.0 - frameWidth) ||
                    localUV.y < frameWidth || localUV.y > (1.0 - frameWidth);
        // Generate a random value for this section
    float sectionRandom = random(vec2(float(sectionX), float(sectionY)));
        // Start with black background
    vec3 finalColor = vec3(0.0);
        // Use smoothstep for smoother transitions
    float step1 = smoothstep(0.125, 0.175, sectionRandom);
    float step2 = smoothstep(0.250, 0.300, sectionRandom);
    float step3 = smoothstep(0.375, 0.425, sectionRandom);
    float step4 = smoothstep(0.500, 0.550, sectionRandom);
    float step5 = smoothstep(0.625, 0.675, sectionRandom);
    float step6 = smoothstep(0.750, 0.800, sectionRandom);
    float step7 = smoothstep(0.875, 0.925, sectionRandom);

    // Mix in each tex with smoother transitions
    if (tex_l > 0) finalColor = mix(finalColor, texture2D(tex[0], uv).rgb, par_a[1]);
    if (tex_l > 1) finalColor = mix(finalColor, texture2D(tex[1], uv).rgb, step1 * par_a[1]);
    if (tex_l > 2) finalColor = mix(finalColor, texture2D(tex[2], uv).rgb, step2 * par_a[1]);
    if (tex_l > 3) finalColor = mix(finalColor, texture2D(tex[3], uv).rgb, step3 * par_a[1]);
    if (tex_l > 4) finalColor = mix(finalColor, texture2D(tex[4], uv).rgb, step4 * par_a[1]);
    if (tex_l > 5) finalColor = mix(finalColor, texture2D(tex[5], uv).rgb, step5 * par_a[1]);
    if (tex_l > 6) finalColor = mix(finalColor, texture2D(tex[6], uv).rgb, step6 * par_a[1]);
    if (tex_l > 7) finalColor = mix(finalColor, texture2D(tex[7], uv).rgb, step7 * par_a[1]);

    // Make frames black
    if (inFrame)
    {
    finalColor = mix(vec3(0.0), finalColor, par_a.w);
    }
    // Output RGB only, alpha is reserved
    gl_FragColor = vec4(finalColor, color.a);
}
