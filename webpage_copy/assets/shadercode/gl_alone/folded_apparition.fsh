precision mediump float;

// Folded Apparition
//
// Repeated spatial inversions gather faint curved structures around a central volume. Their overlapping contributions suggest a folded form suspended in darkness, whose outline and internal density change with the viewing direction.
//
// par_a.x - turns the viewpoint horizontally.
// par_a.y - turns the viewpoint vertically.

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

vec2 mouse = par_a.xy;

float binary(float n, float e)
{
	return n/exp2(e+1.);
}


float gray(float n, float e)
{
	return binary(n,e+1.)+.25;
//	return binary(n,e+1.)+.25;
}


float step_bit(float b)
{
	return step(.5, fract(b));
}


#define PHI (sqrt(5.)*.5-.5)

vec3 hex(float i)
{
	float x = step_bit(gray(i, 0.));
	float y = step_bit(gray(i, 1.));
	float z = step_bit(gray(i, 2.));
	float u = step_bit(gray(i, 3.));
	float v = step_bit(gray(i, 4.));
	float w = step_bit(gray(i, 5.));
	
	#ifdef ANIMATE_DIMENSION
	float t = mod(time * .2, 10.)-5.;
	float l = t > 0. ? fract(t) : fract(1.-t);	
	t 	= abs(t);


	float p = t < 1. ? 0.
		: t < 2. ? mix(0., 1., l)
		: t < 3. ? 1.
		: t < 4. ? mix(1., PHI, l)
		: PHI;
	#else
	float p = mouse.x < .6 
		? mouse.x < .3 
		? 0. : 1. : PHI;
	#endif
	
	return vec3(x * p - u * p + y + v, 
		    y * p - v * p + z + w, 
		    z * p - w * p + x + u) - 1.;
}



vec2 iSphere( in vec3 ro, in vec3 rd, in vec4 sph )//from iq
{
	vec3 oc = ro - sph.xyz;
	float b = dot( oc, rd );
	float c = dot( oc, oc ) - sph.w * sph.w;
	float h = b*b - c;
	if( h<0.0 ) return vec2(-1.0);
	h = sqrt(h);
	vec3 t = hex(h*8192.);
	return t.xy/t.z;
}

vec3 orbit(vec3 p)
{
	return abs(p)/dot(p,p);	
}

float hash(float v)
{
	v *= 1234.5678;
	return fract(v*fract(v));
	
}


float map(in vec3 position) 
{
	const float iterations 	= 4.;
	vec3 origin 		= position;
	float m		= 8.;
	
	float e 		= 0.;
	for (float i = 0.; i < iterations; ++i) 
	{
		
		
		position 		= orbit(position);
		position		= position.yzx - .25;
		position		= position.xzy * 4.;
	
	
		
		float v 			= abs(dot(position, origin));
	        e 			+= exp(-v*m);
	}
	
	return .5*e/iterations;
}

vec3 derivative(vec3 position, float delta)
{
	vec2 offset 	= vec2(delta, 0.);
	vec3 normal 	= vec3(0.);
	normal.x 	= map(position+offset.xyy)-map(position-offset.xyy);
	normal.y 	= map(position+offset.yxy)-map(position-offset.yxy);
	normal.z 	= map(position+offset.yyx)-map(position-offset.yyx);
	return normalize(normal);
}

mat2 rmat(float r)
{
    float c = cos(r);
    float s = sin(r);
    return mat2(c, s, -s, c);
}


vec3 hsv(float h,float s,float v)
{
	return mix(vec3(1.),clamp((abs(fract(h+vec3(3.,2.,1.)/3.)*6.-3.)-1.),0.,1.),s)*v;
}


vec3 view(vec2 pixel, vec3 origin)
{ 
    	vec3 w = normalize( origin );
    	vec3 u = normalize( cross(w,vec3(0.0,1.0,0.0) ) );
    	vec3 v = normalize( cross(u,w));

	float fov = 1.124;
	
	return normalize(pixel.x*u + pixel.y*v + fov * w);
}

void main()
{
	// screen
	vec2 aspect 	= tres/min(tres.x, tres.y);
	vec2 pixel 		= gl_FragCoord.xy/tres.xy;
	vec2 uv	 		= (pixel - .5) * aspect * 2.;
	vec2 mouse 		= (mouse - .5) * aspect * 2.;
    
	
    	// view origin
    	vec3 origin 		= vec3(3.);
	float tau		= 4.*atan(1.);   // using const gives a compiler error!!!
	origin.yz		*= rmat(mouse.y*tau);
    	origin.xz		*= rmat(mouse.x*tau);

	
	//ray position
	vec3 position		= origin;
	
	
	// view direction
    	vec3 direction		= view(uv, origin);

	
   	//bounding sphere
	vec2 bound 		= vec2(3.5, 512.);//iSphere( origin, direction, vec4(0.,0.,0.,	3.5) );
	
	
	// raymarch
	float range 		= 0.;
	float total_range	= bound.x;
	vec3 light		= vec3(mouse * 3., 1.);
	vec3 normal 		= vec3(0.);
	vec3 color_out 		= vec3(0.);

	float decay		= .985;	
	for(float i = 1.; i < 4.; i++)
	{
		total_range	= mix(total_range, exp(range), range*1.)*1.91;   
			
		if (total_range > bound.y) break;
		
		position 	= origin + direction * total_range;
		range 		= abs(map(position));
		
			
		color_out		+= hsv(.4+range/4., range, range*.05)*length(position);
		color_out 		*= decay;

//		normal 		+= derivative(position, range);
	}
//	normal 		= normalize(normal);

	color_out		= pow(color_out+color_out*1.85, vec3(1.5));
	
	gl_FragColor = vec4(color_out, color.a );
}//sphinx