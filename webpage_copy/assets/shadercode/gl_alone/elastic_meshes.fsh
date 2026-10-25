precision mediump float;

// Elastic Meshes
//
// Two grids bend through opposing radial distortions. Their lines overlap into curved diamonds and stretched cells, swelling and relaxing like elastic meshes pulled around neighboring anchors.
//
// par_a.x - moves the first distortion center horizontally and the second vertically.
// par_a.y - moves the first distortion center vertically and the second horizontally.
// par_a.z - increases radial distortion strength.

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

vec2 mouse = par_a.xy;

vec3 sampleColor(vec2 uv, float scale, bool isGray) {
    float pattern = step(min(fract(uv.x * scale), fract(uv.y * scale)), 0.1);
    return isGray ? vec3(pattern * 0.5) : vec3(pattern);
}

vec2 linearToPolarUV(vec2 linearUV, vec2 centerPos, bool inverse) {
    vec2 srcVec = linearUV - centerPos;
    if (inverse) {
        srcVec = -srcVec; // Invert the direction for the second mesh
    }
    
    float r = length(srcVec);
    float distortionStrength = (8.0 + 7.0 * sin(time)) * (0.5 + par_a.z);
    float rSpherical;
    if (inverse) {
        rSpherical = (distortionStrength * r) / (distortionStrength - r * (distortionStrength - 1.0));
    } else {
        rSpherical = (exp(r * log(distortionStrength + 1.0)) - 1.0) / distortionStrength;
    }
    return normalize(srcVec) * rSpherical + centerPos;
}

void main(void) {
    vec2 uv = (gl_FragCoord.xy / tres.xy);
    
    // First mesh (original)
    vec2 uv1 = linearToPolarUV(uv, par_a.xy, false);
    vec3 color1 = sampleColor(uv1, 20.0, false);
    
    // Second mesh (inverse distortion, bigger scale, inverted position)
    // vec2 invertedMousePos = vec2(1.0) - par_a.xy;
    vec2 uv2 = linearToPolarUV(uv, par_a.yx, true);  // was invertedMousePos
    vec3 color2 = sampleColor(uv2, 10.0, true);
    
    // Blend the two patterns
    vec3 finalColor = mix(color1, color2, 0.5);
    
    gl_FragColor = vec4(finalColor, color.a);
}