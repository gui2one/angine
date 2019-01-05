#version 300 es
  
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 t_coords;
uniform vec4 u_color;


out vec4 f_color;
out vec2 f_t_coords;
out vec3 f_normal;
out vec3 f_pos;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{           
	gl_Position =  projection * view * model *vec4(position, 1.0) ;
	
	
	f_pos = vec3(model * vec4(position,1.0));
	f_normal = normal;
	f_normal = mat3(transpose(inverse(model))) * normal;
	f_color = u_color;
	//~ f_color = vec4(tempnormal.x*0.5+0.5 , tempnormal.y*0.5+0.5, tempnormal.z*0.5+0.5 , 1.0);
	
	f_t_coords = t_coords;
	

}


