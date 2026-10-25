precision mediump float;

// Chromatic Windows
//
// A mirrored vid image lies beneath twelve drifting rectangular windows. Within their overlapping panes, the picture changes hue, reverses, or turns to grayscale. The windows slowly resize and fade, composing a collage that never quite settles.
//
// par_a.x - horizontal position of the source region used by the mirrored image.
// par_a.y - window size.
// par_a.z - visibility and number of prominent windows.
// par_a.w - strength of the transformations inside the windows.

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

#define TAU 6.28318530718

float hash11(float n)
{
    return fract(sin(n * 127.1) * 43758.5453);
}

float noise1(float x)
{
    float i = floor(x);
    float f = fract(x);

    f = f * f * (3.0 - 2.0 * f);

    return mix(hash11(i),
               hash11(i + 1.0),
               f);
}

vec3 hueRotate(vec3 c, float angle)
{
    vec3 axis = vec3(0.577350269);

    float co = cos(angle);
    float si = sin(angle);

    return c * co
         + cross(axis, c) * si
         + axis * dot(axis, c) * (1.0 - co);
}

void main(void)
{
    vec2 uv = tcoord;

    // Fixed vertical mirror.
    // Only horizontal video offset via CV.

    float mirroredX =
        0.5 + abs(uv.x - 0.5);

    vec2 videoUV = vec2(
        mirroredX - 0.5 * par_a.x,
        uv.y
    );

    vec3 result =
        texture2D(vid, videoUV).rgb;

    // Continuous time independent of CV.
    float t = time;

    for (int i = 0; i < 12; i++)
    {
        float id = float(i);

        // Individual movement rates:
        // intentionally slow and varied.

        float rateX =
            0.008 + 0.023 *
            hash11(id * 11.7 + 2.1);

        float rateY =
            0.006 + 0.019 *
            hash11(id * 17.3 + 8.4);

        float phaseX =
            TAU * hash11(id * 7.9 + 1.2);

        float phaseY =
            TAU * hash11(id * 13.1 + 5.7);

        // Irregular, smooth drifting.
        // Independent random paths.

        vec2 center = vec2(
            noise1(t * rateX + phaseX),
            noise1(t * rateY + phaseY)
        );

        // Size variation with explicit bounds.

        float widthVariation = noise1(
            t * (0.009 + 0.011 *
            hash11(id * 19.1))
            + id * 23.7
        );

        float heightVariation = noise1(
            t * (0.007 + 0.013 *
            hash11(id * 29.3))
            + id * 41.9
        );

        // Full rectangle widths/heights:
        // minimum 0.06, maximum 0.55
        // in normalized screen coordinates.

        float minSize = 0.06;
        float maxSize = 0.55;

        float sizeBias = par_a.y;

        float width = mix(
            minSize,
            maxSize,
            clamp(
                0.15 + 0.55 * sizeBias
                + 0.30 * (widthVariation - 0.5),
                0.0, 1.0
            )
        );

        float height = mix(
            minSize,
            maxSize,
            clamp(
                0.15 + 0.55 * sizeBias
                + 0.30 * (heightVariation - 0.5),
                0.0, 1.0
            )
        );

        vec2 halfSize =
            0.5 * vec2(width, height);

        // Crisp rectangular edges.

        vec2 d =
            abs(uv - center) - halfSize;

        float mask =
            1.0 - smoothstep(
                -0.0015,
                 0.0015,
                max(d.x, d.y)
            );

        // Independent fade cycles.
        // Fade rate is different per window.

        float fadeRate =
            0.012 + 0.026 *
            hash11(id * 31.7 + 3.4);

        float fadePhase =
            TAU * hash11(id * 37.1 + 9.2);

        float fadeWave =
            0.5 + 0.5 *
            sin(t * fadeRate + fadePhase);

        // Each window has a different
        // activation threshold.
        // Density CV progressively reveals
        // more of the window population.

        float threshold =
            0.15 + 0.65 *
            hash11(id * 43.7 + 6.1);

        float visibility =
            smoothstep(
                threshold - 0.25,
                threshold + 0.20,
                par_a.z + 0.30 * fadeWave
            );

        // Independent strength variation.

        float strengthWave =
            noise1(
                t * (0.009 + 0.016 *
                hash11(id * 47.3))
                + id * 17.1
            );

        float strength =
            par_a.w *
            mix(0.70, 1.0, strengthWave);

        // Three transformation families.

        vec3 transformed = result;

        int effectType = i - (i / 3) * 3;

        if (effectType == 0)
        {
            // Strong hue rotation.

            float hueAngle =
                TAU * (
                    0.20 + 0.60 *
                    noise1(
                        t * 0.012
                        + id * 13.3
                    )
                );

            transformed =
                hueRotate(result, hueAngle);
        }
        else if (effectType == 1)
        {
            // Full color inversion.

            transformed = 1.0 - result;
        }
        else
        {
            // Grayscale of video content,
            // not a solid overlay.

            float gray = dot(
                result,
                vec3(0.299, 0.587, 0.114)
            );

            transformed = vec3(gray);
        }

        // Sequential compositing:
        // overlapping effects accumulate.

        float amount =
            mask * visibility * strength;

        result = mix(
            result,
            transformed,
            amount
        );
    }

    gl_FragColor = vec4(
        clamp(result, 0.0, 1.0),
        color.a
    );
}