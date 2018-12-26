#version 300 es


layout(location = 0) out mediump vec4 color;
in mediump vec2 f_t_coords;
in mediump vec4 f_color;
uniform sampler2D u_tex; //this is the texture
void main()
{           
	//~ color = vec4(1.0,0.8,0.5,1.0);  
	//~ color = f_color;
	color = f_color * texture2D(u_tex,f_t_coords);
	//~ color = texture2D(u_tex,f_t_coords);
}



                                
                                

