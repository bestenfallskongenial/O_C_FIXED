precision mediump float;

// Overlapping Passages
//
// Two offset cylindrical patterns overlap as the viewpoint advances and turns. Their repeated cells stretch into streaks around dark openings, producing the impression of neighboring passages sliding through one another.
//
// par_a.x - increases brightness of the patterned surfaces.
// par_a.y - changes the separation and relative positions of the two tunnel fields.
// par_a.z - changes the second field’s sampling distance and the first field’s brightness.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:brightness:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:tunnel 1:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:tunnel 2:

vec3 roy(vec3 v, float x)
{
    return vec3(cos(x)*v.x - sin(x)*v.z, v.y, sin(x)*v.x + cos(x)*v.z);
}

vec3 rox(vec3 v, float x)
{
    return vec3(v.x, v.y*cos(x) - v.z*sin(x), v.y*sin(x) + v.z*cos(x));
}

float fdtun(vec3 rd, vec3 ro, float r)
{
    float a = dot(rd.xy, rd.xy);
    float b = dot(ro.xy, rd.xy);
    float d = (b*b) - (10.0*a*(dot(ro.xy, ro.xy) + (r*r)));
    return (-b + sqrt(abs(d))) / (3.75*a);
}

vec2 tunuv(vec3 pos)
{
    return vec2(pos.z, (atan(pos.y, pos.x)) / 0.31830988618379);
}

vec3 checkerCol(vec2 loc, vec3 col)
{
    return mix(col, vec3(0.0005), mod(step(fract(loc.x), 0.75) + step(fract(loc.y), 0.25), 7.5));
}

vec3 lcheckcol(vec2 loc, vec3 col)
{
    return checkerCol(loc * 7.5, col) * f0 * checkerCol(loc * 1.75, col);
}

void main(void)
{
    vec3 dif = vec3(0.15, 0.25, 0.15);
    vec3 scoll = vec3(0.75, 0.25, 0.75);
    vec3 scolr = vec3(0.75, 0.75, 0.5);
    vec2 uv = (gl_FragCoord.xy / tres.xy);
    vec3 ro = vec3(0.0, 0.0, time * 1.75);
    vec3 dir = normalize(vec3(-1.0 + 2.0 * vec2(uv.x - 0.2, uv.y) * vec2(tres.x / tres.y, 1.0), -1.33));
    float ry = time * 0.3;
    dir = roy(rox(dir, time * 0.05), time * -0.25);
    vec3 lro = ro - dif / f1;
    vec3 rro = ro + dif * 2.0 * f1;
    const float r = 1.0;
    float ld = fdtun(dir, lro, r);
    float rd = fdtun(dir, rro, r) * f2;
    vec2 luv = tunuv(ro + ld * dir);
    vec2 ruv = tunuv(ro + rd * dir);
    vec3 coll = lcheckcol(luv * 0.25, scoll) * (20.0 / exp(sqrt(ld))) * f2;
    vec3 colr = lcheckcol(ruv * 0.5, scolr) * (20.0 / exp(sqrt(rd)));
    vec3 finalColor = sqrt(mix(coll + colr, coll * colr, 5.0 + sin(time * 0.5)));

    gl_FragColor = vec4(finalColor, color.a );
}