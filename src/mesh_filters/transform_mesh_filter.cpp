#include "transform_mesh_filter.h"


TransformMeshFilter::TransformMeshFilter()
	: MeshFilter()
{
	setType(TRANSFORM_MESH_FILTER);
	
	p_menu = new ParamMenu(
			"transform order", 
			std::vector<std::string>{
					"Rot/Scale/Trans","Scale/Rotate/Trans","Scale/Trans/Rotate"});
	param_layout.push(p_menu);

	p_translate = new ParamVec3("translate",glm::vec3( 0.0f, 0.0f, 0.0f),"t_");
	param_layout.push(p_translate);
	
	p_rotate = new ParamVec3("rotate",glm::vec3( 0.0f, 0.0f, 0.0f),"r_");
	param_layout.push(p_rotate);	
	
	p_scale = new ParamVec3("scale",glm::vec3( 1.0f, 1.0f, 1.0f),"s_");
	param_layout.push(p_scale);	
}

TransformMeshFilter::TransformMeshFilter(const TransformMeshFilter& other):MeshFilter(other){
	
	p_menu = new ParamMenu(*other.p_menu);
	param_layout.push(p_menu);

	p_translate = new ParamVec3(*other.p_translate);
	param_layout.push(p_translate);
	
	p_rotate = new ParamVec3(*other.p_rotate);
	param_layout.push(p_rotate);	
	
	p_scale = new ParamVec3(*other.p_scale);
	param_layout.push(p_scale);		
}

Mesh TransformMeshFilter::applyFilter(Mesh & source_mesh)
{
	Mesh mesh;
	mesh = source_mesh;
	
	glm::mat4 matrix = glm::mat4(1.0f);
	
	glm::vec3 translate = p_translate->getValue();
	glm::vec3 rotate = p_rotate->getValue();
	glm::vec3 scale = p_scale->getValue();

	if(p_menu->current_choice == 0){
			  
		matrix = glm::translate(matrix, p_translate->getValue());
		
		matrix = glm::scale(matrix , p_scale->getValue());		
		
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().x), glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().z), glm::vec3(0.0f, 0.0f, 1.0f));

	}else if(p_menu->current_choice == 1){		
		
		matrix = glm::translate(matrix, p_translate->getValue());			
		
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().x), glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().z), glm::vec3(0.0f, 0.0f, 1.0f));
		
		matrix = glm::scale(matrix , p_scale->getValue());	
		
	}else if(p_menu->current_choice == 2){
		
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().x), glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians( p_rotate->getValue().z), glm::vec3(0.0f, 0.0f, 1.0f));			
		
		matrix = glm::translate(matrix, p_translate->getValue());	
		
		matrix = glm::scale(matrix , p_scale->getValue());			
	}
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		mesh.vertices[i].position = matrix * glm::vec4(mesh.vertices[i].position,1.0f);
	}	
	
	
	
	
	//~ for (int i = 0; i < mesh.vertices.size(); i++)
	//~ {
		//~ mesh.vertices[i].position.x += p_translate->getValue().x;
		//~ mesh.vertices[i].position.y += p_translate->getValue().y;
		//~ mesh.vertices[i].position.z += p_translate->getValue().z;
	//~ }
	//~ 
	//~ glm::mat4 rot_matrix = glm::mat4(1.0f);
	//~ rot_matrix = glm::rotate(rot_matrix, glm::radians( p_rotate->getValue().x), glm::vec3(1.0f, 0.0f, 0.0f));
	//~ rot_matrix = glm::rotate(rot_matrix, glm::radians( p_rotate->getValue().y), glm::vec3(0.0f, 1.0f, 0.0f));
	//~ rot_matrix = glm::rotate(rot_matrix, glm::radians( p_rotate->getValue().z), glm::vec3(0.0f, 0.0f, 1.0f));
	//~ 
	//~ 
	//~ rot_matrix = glm::scale(rot_matrix, p_scale->getValue());
	//~ for (int i = 0; i < mesh.vertices.size(); i++)
	//~ {
		//~ mesh.vertices[i].position = rot_matrix * glm::vec4(mesh.vertices[i].position,1.0f);
	//~ }
	

	mesh_cache = mesh;
	return mesh;
};


