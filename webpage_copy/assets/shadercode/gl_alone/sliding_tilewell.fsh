precision mediump float;

// Sliding Tilewell
//
// Rows of small squares slide at different speeds through a perspective-like warp. Their sizes and spacing gather toward a movable convergence region, while individual tiles shrink and reappear like fragments traveling through a shallow tiled well.
//
// par_a.x - increases the variation in tile shrinkage.
// par_a.y - moves the warp center diagonally, changing perspective and tile density.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:accent lines:
float f1 = mix(0.2, 0.8, par_a[1]);     //f1:spatial pos:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:squeeze:

vec2 mouse = par_a.xy;

vec4 hash42(vec2 p)
{
	vec4 p4 = fract(p.xyxy * vec4(.1031, .1030, .0973, .1099));
    p4 += dot(p4, p4.wzxy+33.33);
    return fract((p4.xxyz+p4.yzzw)*p4.zywx);
}

void mainImage( out vec4 O, in vec2 F)
{
    // coord setup
    vec2 u = (F/tres.xy);
    u.x *= tres.x/tres.y;
    u.y+=.5;
    u *= .5;
    
    float t = time *0.3;

    vec2 c = floor(u); // square cell id (basically ~0,1 whole viewport)
    vec2 p = u-c; // 2D position in cell
    float l = length(p-mouse.y); // length from center
    u /= 2.*max(0.0001,l/7.4); // warp uv based on length accounting for Z
    u += vec2(u.y,-u.x); // skew coords to correct prev op so we can manip z directly
    vec2 id = floor(u); // now find a new cell based on new coords.
    vec4 h = hash42(id.yy); // get random row value
    u.x += ((mod(id.y,2.)+1.))*t*sign(h.z-.5)*3.+(3.*(h.y-.5));// slide rows randomly
    u.x += t*13.; // move forward
    id = floor(u); // recalc cell after sliding
    h = hash42(id); // hash of this cell
    vec2 idp = u-id; // position in the new cell
    float d = .45-max(abs(idp.x-.5),abs(idp.y-.5)); // distance to edge
    d = step(h.a* (sin(3.*t*h.r)*.5+.5)*mouse.x, d); // stylize squares
    
    O = h * d * l * 2.;
}

void main(void)
{
    mainImage(gl_FragColor, gl_FragCoord.xy);
    gl_FragColor.a = 1.;
}