precision mediump float;

// Field Pendulums
//
// Several moving attraction centers bend a directional field into paired lobes and sweeping fans. Their boundaries pivot, stretch, and gather along a shifting axis, while a breathing view reveals either a broad fold or a chain of smaller pendulum-like forms.
//
// par_a.x - shifts the repeating tonal bands around the field directions.
// par_a.y - widens the horizontal view on landscape displays.
// par_a.z - increases spacing between the moving attraction centers.

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

float f0 = mix(0.05, 0.95, par_a[0]);   //f0:twist:
float f1 = mix(0.05, 0.95, par_a[1]);   //f1:stretch:
float f2 = mix(0.05, 0.95, par_a[2]);   //f2:zoom:

#define CT_N 2

struct ct_axes 
{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
};

struct ct_plane2d 
{
    ct_axes axes;
    float xstep;
    float ystep;
};

ct_axes ct_axes_from_radius(in vec3 vpcircle)
{
    return ct_axes(
        vpcircle.x - vpcircle.z,
        vpcircle.x + vpcircle.z,
        vpcircle.y - vpcircle.z,
        vpcircle.y + vpcircle.z
    );
}

ct_plane2d ct_plane2d_create(in ct_axes axes)
{
    float awidth = axes.xmax - axes.xmin;
    float aheight = axes.ymax - axes.ymin;
    
    float daspect = abs(tres.y / tres.x);
    float waspect = abs(aheight / awidth);
    
    if (daspect > waspect) 
        {
        float excess = aheight * (daspect / waspect - 1.0);
        axes.ymax += excess / 2.0;
        axes.ymin -= excess / 2.0;
        }
    else if (daspect < waspect) 
        {
        float excess = awidth * (waspect / daspect - 1.0);
        axes.xmax += excess / 0.5 * f1;
        axes.xmin -= excess / 0.5 * f1; 
        }
    
    return ct_plane2d   (   axes,
                        (   axes.xmax - axes.xmin   ) / tres.x,
                        (   axes.ymax - axes.ymin   ) / tres.y  );
}

vec2 ct_plane2d_project(in ct_plane2d self, in vec2 z)
{
   return vec2  (   self.axes.xmin + z.x * self.xstep,
                    self.axes.ymin + z.y * self.ystep   );
}

vec2 ct_vfield_normal(in vec2 p, float npow)
{
    vec2 g = vec2(0.0, 0.0);
    
    float abase = 6.28318 / float(CT_N);
    float rbase = 1.0 / float(CT_N) * (f2 + 1.0);
    
    for (int i = 0; i < CT_N * 2; ++i) 
        {
        float angle = abase * float(i);
        float radius = rbase * float(i);
        
        angle += sin(time * 0.5) * 3.1459;
       
        vec2 vp = vec2  (   cos(angle) * radius * (1.0 + abs(sin(time))), 
                            sin(angle) * radius * (1.0 + abs(cos(time)))    );
        
        float vm = -1.0;
        
        vec2 dif = vp - p;
        float sum = dif[0] * dif[0] + dif[1] * dif[1];
        float mass = pow(sum, npow);
        
        g[0] = g[0] + vm * dif[0] / mass;
        g[1] = g[1] + vm * dif[1] / mass;
        }
    return normalize(g);
}

float ct_normal_pi(in vec2 z, in float sa)
{
    float a = atan(z[1], z[0]) + sa;
    if (a < 0.0) a += 6.28318;
    a /= 6.28318;
    return a;
}

vec4 ct_vpixel(in vec2 z, in vec2 c, in int n, in float npow)
{
    vec2 vn = ct_vfield_normal(z, npow);
    
    float a = cos(time * 0.1) * 3.14;
    
    float npi = ct_normal_pi(vn, a);
    
    float scale = float(CT_N);
    
    float color_out = mod(npi * scale + f0, 1.0);
    
    if (color_out < 0.5) 
        {
        color_out /= 0.5;
        return vec4(color_out, 0.0, color_out, color.a);
        }
    return vec4(color_out, color_out, color_out, color.a);
}

vec4 ct_main(in ct_plane2d plane, in vec2 c)
{  
    return ct_vpixel(c, c, 128, 2.0);
}

void main(void)
{
    vec3 vpcircle = vec3    (   0.0, 0.0, 0.1 + abs(sin(time * 0.1)) * 4.9    );
    
    ct_plane2d plane = ct_plane2d_create( ct_axes_from_radius(vpcircle) );
    
    vec2 c = ct_plane2d_project(plane, gl_FragCoord.xy);
    
    vec4 color_out = ct_main(plane, c);
    
    gl_FragColor = color_out;
}