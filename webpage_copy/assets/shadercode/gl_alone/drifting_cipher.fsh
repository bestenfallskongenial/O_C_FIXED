precision mediump float;

// Drifting Cipher
//
// Rows of small, invented glyphs scroll diagonally across the screen. Uneven subdivisions break the lettering into patches of different sizes, while staggered reveals make the marks appear like fragments of a message passing through a moving veil.
//
// par_a.x - increases glyph size and spacing.
// par_a.y - changes the internal grid of strokes that forms each glyph.
// par_a.z - increases variation in the timing of the glyphs’ appearance.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:shape:
float f1 = mix(0.15, 0.95, par_a[1]);   //f1:size:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:fill:

#define PI 3.1415926535

float random2d(vec2 n)
{ 
    return fract(sin(dot(n, vec2(12111.9898, 4.1414))) * 2398.5453);
}

vec2 getCellIJ(vec2 uv, float gridDims)
{
    return floor(uv * gridDims) / gridDims;
}

vec2 rotate2D(vec2 position, float theta)
{
    mat2 m = mat2(cos(theta), -sin(theta), sin(theta), cos(theta));
    return m * position;
}

float letter(vec2 coord, float size)
{
    vec2 gp = floor(coord / size * 7.);
    vec2 rp = floor(fract(coord / size) * 7. * f1);
    vec2 odd = fract(rp * 0.5) * 2.;
    float rnd = random2d(gp);
    float c = max(odd.x, odd.y) * step(0.5, rnd);
    c += min(odd.x, odd.y);
    c *= rp.x * (6. - rp.x);
    c *= rp.y * (6. - rp.y);
    return clamp(c, 0., 1.);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / tres.xy;    
    uv.x *= tres.x / tres.y;

    float scrollSpeed = 0.2;
    float dims = 2.0;
    const int maxSubdivisions = 6;
    
    uv = rotate2D(uv, PI / 6.);
    uv.y -= time * scrollSpeed;
    
    float cellRand;
    vec2 ij;
    
    for(int i = 0; i <= maxSubdivisions; i++) 
        { 
        ij = getCellIJ(uv, dims);
        cellRand = random2d(ij);
        dims *= 2.0;
        float cellRand2 = random2d(ij + 454.4543);
        if (cellRand2 > 0.3) 
            {
            break; 
            }
        }
   
    float b = letter(uv, 1.0 / (dims) * f0);
   
    float scrollPos = time * scrollSpeed + 0.5;
    float showPos = -ij.y + cellRand * f2;
    float fade = smoothstep(showPos, showPos + 0.05, scrollPos);
    b *= fade;
    
    gl_FragColor = vec4(vec3(b), color.a);
}