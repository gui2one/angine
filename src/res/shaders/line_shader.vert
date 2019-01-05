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
uniform vec3 u_camera_pos;



void main()
{           
	vec3 camera_dir = normalize(u_camera_pos - position);
	gl_Position =  projection * view * model *vec4(position, 1.0); // + vec4(camera_dir * 0.001,1.0);
	

	f_color = vec4(1.0,1.0,1.0,1.0);
	f_color *= u_color;
	
	f_t_coords = t_coords;


}


