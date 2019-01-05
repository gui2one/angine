#version 300 es


layout(location = 0) out mediump vec4 color;
in mediump vec2 f_t_coords;
in mediump vec4 f_color;

in mediump vec3 f_normal;
in mediump vec3 f_pos;
uniform sampler2D u_tex; //this is the texture

mediump vec3 norm;
mediump vec3 lightDir;

void main()
{          
	
	norm = normalize(f_normal);
	lightDir = normalize(vec3(-2.0,-2.0,2.0) - f_pos);   
	//~ color = vec4(0.0,0.8,0.5,1.0);  
	
	mediump float diff = max(dot(norm, lightDir), 0.0);
	
	color = vec4(diff, diff, diff,1.0) * f_color;
	//~ color = f_color * texture2D(u_tex,f_t_coords);
	//~ color = texture2D(u_tex,f_t_coords);
}



                                
                                

