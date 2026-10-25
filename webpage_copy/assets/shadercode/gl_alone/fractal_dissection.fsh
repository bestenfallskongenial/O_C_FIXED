precision mediump float;

// FRACTAL DISSECTION
//
// A slowly rotating ellipsoid is cut by a movable plane. Its smooth outer shell gives way to a patterned cross-section, where nested bands and fine veins suggest an intricate structure hidden beneath the surface.
//
// par_a.x - cutting-plane position.
// par_a.y - cutting-plane tilt.
// par_a.z - fractal power shaping the exposed interior pattern.
// par_a.w - viewing rotation offset through a full turn.

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

#define PI 3.14159265359


float fractalField(vec3 c, float power)
{
    vec3 z = vec3(0.0);
    float orbit = 10.0;

    // Deliberately truncated Mandelbulb.
    // Three iterations, no nested loops.

    for (int i = 0; i < 3; i++)
    {
        float r = length(z);

        orbit = min(orbit, r);

        if (r > 2.0)
            break;

        float theta = acos(
            clamp(
                z.z / max(r, 0.00001),
                -1.0, 1.0
            )
        );

        float phi = atan(z.y, z.x);

        float zr = pow(r, power);

        theta *= power;
        phi *= power;

        z = zr * vec3(
            sin(theta) * cos(phi),
            sin(theta) * sin(phi),
            cos(theta)
        ) + c;
    }

    return length(z);
}

void main(void)
{
    vec2 uv = tcoord;

    vec2 p = uv * 2.0 - 1.0;
    p.x *= tres.x / max(tres.y, 1.0);


    vec3 ro = vec3(0.0, 0.0, 3.2);
    vec3 rd = normalize(vec3(p, -2.3));


    float angle =
        time * 0.08 +
        par_a.w * 6.2831853;

    float ca = cos(angle);
    float sa = sin(angle);

    ro = vec3(
        ca * ro.x - sa * ro.z,
        ro.y,
        sa * ro.x + ca * ro.z
    );

    rd = vec3(
        ca * rd.x - sa * rd.z,
        rd.y,
        sa * rd.x + ca * rd.z
    );


    vec3 scale = vec3(
        0.78,
        1.12,
        0.72
    );

    vec3 o = ro / scale;
    vec3 d = rd / scale;

    float A = dot(d, d);
    float B = 2.0 * dot(o, d);
    float C = dot(o, o) - 1.0;

    float discriminant =
        B * B - 4.0 * A * C;


    vec3 result = vec3(
        0.012,
        0.018,
        0.035
    );

    if (discriminant > 0.0)
    {
        float root = sqrt(discriminant);

        float tNear =
            (-B - root) / (2.0 * A);

        float tFar =
            (-B + root) / (2.0 * A);

        if (tFar > 0.0)
        {

            float tilt =
                (par_a.y - 0.5) * PI;

            vec3 planeNormal = normalize(
                vec3(
                    sin(tilt),
                    0.25,
                    cos(tilt)
                )
            );

            float planeOffset = mix(
                -1.1,
                1.1,
                par_a.x
            );

            // Keep points satisfying:
            // dot(p,n) <= planeOffset

            float denom = dot(
                rd,
                planeNormal
            );

            float planeT = 10000.0;

            if (abs(denom) > 0.00001)
            {
                planeT =
                    (planeOffset -
                     dot(ro, planeNormal))
                    / denom;
            }


            float surfaceT = max(
                tNear,
                0.0
            );

            vec3 surfacePoint =
                ro + rd * surfaceT;

            float side = dot(
                surfacePoint,
                planeNormal
            ) - planeOffset;

            bool cutVisible = false;
            bool bodyVisible = false;

            if (side <= 0.0 && tNear > 0.0)
            {
                bodyVisible = true;
            }
            else if (
                planeT > max(tNear, 0.0) &&
                planeT < tFar &&
                denom < 0.0
            )
            {
                cutVisible = true;
                surfaceT = planeT;
            }
            else if (
                tFar > max(tNear, 0.0)
            )
            {
                vec3 backPoint =
                    ro + rd * tFar;

                if (dot(
                    backPoint,
                    planeNormal
                ) <= planeOffset)
                {
                    bodyVisible = true;
                    surfaceT = tFar;
                }
            }


            if (bodyVisible)
            {
                vec3 hit =
                    ro + rd * surfaceT;

                vec3 normal = normalize(
                    hit / (scale * scale)
                );

                vec3 light = normalize(
                    vec3(-0.5, 0.8, 1.0)
                );

                float diffuse = max(
                    dot(normal, light),
                    0.0
                );

                float rim = pow(
                    1.0 - abs(
                        dot(normal, -rd)
                    ),
                    3.0
                );

                vec3 material = vec3(
                    0.19,
                    0.09,
                    0.27
                );

                result = material *
                    (0.22 + diffuse * 1.15);

                result += rim * vec3(
                    0.42,
                    0.18,
                    0.65
                );


                float grain =
                    sin(hit.y * 26.0 +
                        sin(hit.x * 12.0) * 2.0);

                result *=
                    0.9 + 0.1 * grain;
            }


            if (cutVisible)
            {
                vec3 hit =
                    ro + rd * surfaceT;

                float power = mix(
                    3.0,
                    9.0,
                    par_a.z
                );

                float f = fractalField(
                    hit * 1.45,
                    power
                );


                float structure =
                    log(1.0 + f);

                float bands =
                    sin(structure * 18.0);

                float veins =
                    sin(structure * 43.0);


                vec3 deep = vec3(
                    0.015,
                    0.035,
                    0.055
                );

                vec3 tissue = vec3(
                    0.65,
                    0.18,
                    0.12
                );

                vec3 mineral = vec3(
                    0.95,
                    0.68,
                    0.32
                );

                float pattern =
                    0.5 + 0.5 * bands;

                result = mix(
                    deep,
                    tissue,
                    pattern
                );

                result = mix(
                    result,
                    mineral,
                    pow(
                        0.5 + 0.5 * veins,
                        8.0
                    ) * 0.75
                );


                float facing = abs(
                    dot(
                        planeNormal,
                        -rd
                    )
                );

                result *=
                    0.45 + facing * 0.75;


                float ellipsoidRadius =
                    length(hit / scale);

                float edge = smoothstep(
                    0.91,
                    1.0,
                    ellipsoidRadius
                );

                result += edge *
                    vec3(0.9, 0.45, 0.22);
            }
        }
    }

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}
