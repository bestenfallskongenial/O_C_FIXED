precision mediump float;

// Octahedral Afterimage
//
// A faceted octahedron turns through space, its triangular surfaces catching light and revealing bands or fragments of vid. Outside its current silhouette, bfr preserves shrinking traces of earlier orientations.
//
// par_a.x - rotation speed.
// par_a.y - octahedron size.
// par_a.z - blends the surface material toward video texture.
// par_a.w - persistence of the backbuffer afterimages.

varying vec2 tcoord;

uniform float time;
uniform vec2 tres;

uniform vec4 seed;
uniform vec4 audio;
uniform vec4 color;
uniform vec4 par_a;
uniform vec4 par_b;

uniform sampler2D tex[8];
uniform int tex_l;

uniform sampler2D bfr;
uniform sampler2D vid;

#define FAR 10000.0

// Intersect a ray with a pair of
// parallel planes: -h <= dot(n,p) <= h.

vec2 slab(
    vec3 ro,
    vec3 rd,
    vec3 n,
    float h
)
{
    float a = dot(ro, n);
    float b = dot(rd, n);

    if (abs(b) < 0.00001)
    {
        if (abs(a) > h)
            return vec2(FAR, -FAR);

        return vec2(-FAR, FAR);
    }

    float t0 = (-h - a) / b;
    float t1 = ( h - a) / b;

    return vec2(
        min(t0, t1),
        max(t0, t1)
    );
}

void main(void)
{
    vec2 uv = tcoord;

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / max(tres.y, 1.0);

    // Four CV-controlled dimensions.
    float spin = 0.08 + par_a.x * 1.8;
    float extent = 0.45 + par_a.y * 0.75;
    float textureMix = par_a.z;
    float persistence = par_a.w;

    float t = time * spin;


    vec3 ro = vec3(0.0, 0.0, 3.5);
    vec3 rd = normalize(vec3(p, -2.2));


    float cy = cos(t);
    float sy = sin(t);

    float cx = cos(t * 0.73);
    float sx = sin(t * 0.73);

    // Y rotation
    ro = vec3(
        cy * ro.x - sy * ro.z,
        ro.y,
        sy * ro.x + cy * ro.z
    );

    rd = vec3(
        cy * rd.x - sy * rd.z,
        rd.y,
        sy * rd.x + cy * rd.z
    );

    // X rotation
    ro = vec3(
        ro.x,
        cx * ro.y - sx * ro.z,
        sx * ro.y + cx * ro.z
    );

    rd = vec3(
        rd.x,
        cx * rd.y - sx * rd.z,
        sx * rd.y + cx * rd.z
    );


    vec3 n0 = vec3( 1.0,  1.0,  1.0);
    vec3 n1 = vec3( 1.0,  1.0, -1.0);
    vec3 n2 = vec3( 1.0, -1.0,  1.0);
    vec3 n3 = vec3(-1.0,  1.0,  1.0);

    vec2 s0 = slab(ro, rd, n0, extent);
    vec2 s1 = slab(ro, rd, n1, extent);
    vec2 s2 = slab(ro, rd, n2, extent);
    vec2 s3 = slab(ro, rd, n3, extent);

    float nearT = max(
        max(s0.x, s1.x),
        max(s2.x, s3.x)
    );

    float farT = min(
        min(s0.y, s1.y),
        min(s2.y, s3.y)
    );


    vec2 center = vec2(0.5);

    vec2 historyUV =
        center + (uv - center) * 1.008;

    vec3 history = texture2D(
        bfr,
        clamp(historyUV, 0.0, 1.0)
    ).rgb;

    vec3 result = history *
        mix(0.55, 0.985, persistence);


    if (farT >= max(nearT, 0.0))
    {
        float hitT = max(nearT, 0.0);

        vec3 hit = ro + rd * hitT;


        vec3 normal = n0;

        if (s1.x > s0.x &&
            s1.x >= s2.x &&
            s1.x >= s3.x)
        {
            normal = n1;
        }

        if (s2.x > s0.x &&
            s2.x > s1.x &&
            s2.x >= s3.x)
        {
            normal = n2;
        }

        if (s3.x > s0.x &&
            s3.x > s1.x &&
            s3.x > s2.x)
        {
            normal = n3;
        }

        if (dot(normal, hit) < 0.0)
            normal = -normal;

        normal = normalize(normal);


        vec2 faceUV = vec2(
            dot(hit, vec3(1.0, -1.0, 0.0)),
            dot(hit, vec3(0.0, 1.0, -1.0))
        );

        faceUV =
            faceUV * (0.5 / extent) + 0.5;

        vec3 video = texture2D(
            vid,
            clamp(faceUV, 0.0, 1.0)
        ).rgb;


        vec3 lightDir = normalize(
            vec3(0.6, 0.9, 1.2)
        );

        float diffuse = max(
            dot(normal, lightDir),
            0.0
        );

        float lighting =
            0.18 + 0.82 * diffuse;


        vec3 cell = abs(hit) / extent;

        float edgeDistance = min(
            min(cell.x, cell.y),
            cell.z
        );

        float edge = 1.0 - smoothstep(
            0.012,
            0.045,
            edgeDistance
        );

        float bands = 0.5 + 0.5 * sin(
            hit.x * 24.0 +
            hit.y * 17.0 +
            hit.z * 13.0
        );

        vec3 material = mix(
            vec3(
                0.08 + bands * 0.16,
                0.32 + bands * 0.25,
                0.48 + bands * 0.35
            ),
            video,
            textureMix
        );

        material *= lighting;

        material += edge * vec3(
            0.8, 0.9, 1.0
        );


        result = material;
    }

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
