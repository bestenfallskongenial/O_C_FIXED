precision mediump float;

// Flow Field Transform
//
// Brightness in a source texture defines a field of directions that carries bfr through the frame. Gentle rotation bends the currents, while fresh texture deposits feed the trails and keep the flowing picture alive.
//
// par_a.x - rotation speed and direction of the transport field; the midpoint stops its time-driven rotation.
// par_a.y - transport distance per frame.
// par_a.z - global feedback swirl; the midpoint removes the swirl.
// par_a.w - persistence of the transported imagery.

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Screen resolution

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];
uniform sampler2D bfr;

void main(void)
{
    vec2 uv = gl_FragCoord.xy / tres.xy;
    vec2 px = 1.0 / tres.xy;

    vec4 src = texture2D(tex[0], uv);

    float field = dot(src.rgb, vec3(0.299, 0.587, 0.114));

    float angle = field * 6.2831853 + time * mix(-2.0, 2.0, par_a.x);
    float radius = mix(0.5, 8.0, par_a.y);

    vec2 flow = vec2(cos(angle), sin(angle)) * px * radius;

    vec2 center = uv - 0.5;

    float swirl = mix(-0.03, 0.03, par_a.z);
    vec2 spin;
    spin.x = center.x - center.y * swirl;
    spin.y = center.y + center.x * swirl;

    vec2 buv = spin + 0.5 - flow;

    vec4 old = texture2D(bfr, buv);

    float decay = mix(0.965, 0.999, par_a.w);

    vec3 inject = src.rgb * field * 0.035;

    vec3 rgb = old.rgb * decay + inject;

    gl_FragColor = vec4(rgb, color.a);
}