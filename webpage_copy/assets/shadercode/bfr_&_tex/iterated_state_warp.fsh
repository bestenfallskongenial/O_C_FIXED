precision mediump float;

// Iterated State Warp
//
// bfr stores a directional field that transports and reshapes itself. Edges from a source texture push the field into new currents, while the displayed channels reveal its direction and strength as an evolving abstract surface.
//
// par_a.x - distance over which the stored field transports itself.
// par_a.y - strength of the forces introduced by source-texture edges.
// par_a.z - persistence of the stored directional field.
// par_a.w - strength of the source-image contribution.

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

    /*
        Read previous state.
        bfr.rg is treated as a stored displacement vector.
        Neutral value is 0.5 / 0.5.
    */
    vec4 old = texture2D(bfr, uv);

    vec2 state = old.rg * 2.0 - 1.0;

    /*
        Feed the state back into its own lookup.
        This makes the buffer recursively deform itself.
    */
    float strength = mix(4.0, 80.0, par_a.x);
    vec2 buv = uv - state * px * strength;

    vec4 prev = texture2D(bfr, buv);
    vec2 prevState = prev.rg * 2.0 - 1.0;

    /*
        New force from tex[0].
        This is only an input force, not the final picture.
    */
    float lum = dot(src.rgb, vec3(0.299, 0.587, 0.114));

    vec2 grad;
    grad.x = dot(texture2D(tex[0], uv + vec2(px.x, 0.0)).rgb, vec3(0.299, 0.587, 0.114)) -
             dot(texture2D(tex[0], uv - vec2(px.x, 0.0)).rgb, vec3(0.299, 0.587, 0.114));

    grad.y = dot(texture2D(tex[0], uv + vec2(0.0, px.y)).rgb, vec3(0.299, 0.587, 0.114)) -
             dot(texture2D(tex[0], uv - vec2(0.0, px.y)).rgb, vec3(0.299, 0.587, 0.114));

    float force = mix(0.0, 2.5, par_a.y);

    vec2 nextState = prevState * mix(0.80, 0.995, par_a.z) + grad * force;

    nextState = clamp(nextState, -1.0, 1.0);

    /*
        Display derived from accumulated state, not directly from tex[0].
    */
    float energy = length(nextState);

    vec3 stateColor = vec3(
        nextState.x * 0.5 + 0.5,
        nextState.y * 0.5 + 0.5,
        energy
    );

    vec3 inject = src.rgb * lum * mix(0.0, 0.25, par_a.w);

    vec3 rgb = mix(stateColor, stateColor + inject, par_a.w);

    gl_FragColor = vec4(rgb, color.a);
}