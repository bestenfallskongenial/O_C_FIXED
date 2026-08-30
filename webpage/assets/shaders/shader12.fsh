//  Octree Traversal Shader                                   
//----------------------------------------// shader name length of 40 characters

precision mediump float;

varying vec2 tcoord;

uniform float time;                     // System time for animation (in seconds)
uniform vec2 tres;                      // Center of the screen

uniform vec4 seed;                      // random value between 0 and 1
uniform vec4 audio;                     // Low, Mid, Hi energy average
uniform vec4 color;
uniform vec4 par_a;                     // Normalised ( 0.0 to 1.0 )
uniform vec4 par_b;                     // Normalised ( 0.0 to 1.0 )

uniform sampler2D tex[8];               // Array of texture samplers       
uniform int tex_l;                      // number of loaded texures 
//f0:rotate one:
//f1:rotate two:
//f2:pull zoom:
float f0 = mix(0.05, 0.95, par_a[0]);
float f1 = mix(0.05, 0.95, par_a[1]);
float f2 = mix(0.05, 0.95, par_a[2]);

#define detail 1
#define steps 2 // or 4??

#define rot(spin) mat2(cos(spin),sin(spin),-sin(spin),cos(spin))

float rnd(vec4 v){ return fract(sin(v.x)*43758.5453); }

int getvoxel(vec3 p, float size) {
    if (p.x == 0.0 && p.y == 0.0) {
        return 0;
    }
    
    float val = rnd(vec4(p,size));
    return int(val*val*3.0);
}

vec3 voxel(vec3 ro, vec3 rd, float size) {
    size *= 0.5;
    vec3 hit = -(sign(rd)*(ro-size)-size)/max(abs(rd),0.001);
    return hit;
}

void main() {
    vec2 uv = (gl_FragCoord.xy * 2.0 - tres.xy) / tres.y / vec2(f2);
    float size = 1.0;
    
    vec3 ro = vec3(0.5+sin(time)*0.4, 0.5+cos(time)*0.4, time);
    vec3 rd = normalize(vec3(uv,1.0));
    
    rd.xz *= rot(f0/tres.x*3.14-3.14*f0);
    rd.yz *= rot(f1/tres.y*3.14-3.14*f1);
    
    vec3 lro = mod(ro,size);
    vec3 fro = ro-lro;
    vec3 mask;
    bool exitoct = false;
    int recursions = 0;
    float dist = 0.0;
    int i = 0;
    float edge = 1.0;
    
    for (int i = 0; i < steps; i++) {
        int voxelstate = getvoxel(fro,size);
        
        if (exitoct) {
            vec3 newfro = floor(fro/size*0.5)*size*2.0;
            
            lro += fro-newfro;
            fro = newfro;
            
            recursions--;
            size *= 2.0;
            
            exitoct = (recursions > 0) && (abs(dot(mod(fro/size+0.5,2.0)-1.0+mask*sign(rd)*0.5,mask))<0.1);
        }
        else if(voxelstate == 1) {
            if(recursions>detail) break;
            
            recursions++;
            size *= 0.5;
            
            vec3 mask2 = step(vec3(size),lro);
            fro += mask2*size;
            lro -= mask2*size;
        }
        else if (voxelstate == 0) {
            vec3 hit = voxel(lro, rd, size);
            if (hit.x < min(hit.y,hit.z)) {
                mask = vec3(1,0,0);
            } else if (hit.y < hit.z) {
                mask = vec3(0,1,0);
            } else {
                mask = vec3(0,0,1);
            }
            float len = dot(hit,mask);
            
            ro += rd*len;
            lro += rd*len-mask*sign(rd)*size;
            vec3 newfro = fro+mask*sign(rd)*size;
            exitoct = (floor(newfro/size*0.5+0.25)!=floor(fro/size*0.5+0.25))&&(recursions>0);
            fro = newfro;
        }
        else {
            break;
        }
    }
    
    if(i < steps) {
        float val = fract(dot(fro,vec3(15.23,754.345,3.454)));
        vec3 normal = mask*sign(rd);
        vec3 color_out = sin(val*vec3(39.896,57.3225,48.25))*0.5+0.5;
        vec3 finalColor = color_out*(normal*0.25+0.75);
        
        vec3 q = abs(lro/size-0.5)*(1.0-mask);
        edge = clamp(-(max(max(q.x,q.y),q.z)-0.5)*20.0*size,0.0,edge);
        finalColor *= edge;
        
        gl_FragColor = vec4(finalColor, color.a);
    } else {
        gl_FragColor = vec4(0.0, 0.0, 0.0, color.a);
    }
}