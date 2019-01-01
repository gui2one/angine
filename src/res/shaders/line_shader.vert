#version 300 es
  
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 t_coords;
uniform vec4 u_color;


out vec4 f_color;
out vec2 f_t_coords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;



void main()
{           
	gl_Position =  projection * view * model *vec4(position, 1.0) ;

	f_color = vec4(1.0,1.0,1.0,1.0);
	f_color *= u_color;
	
	f_t_coords = t_coords;


}

