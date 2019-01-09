#include "duplicate_mesh_filter.h"


DuplicateMeshFilter::DuplicateMeshFilter()
	: MeshFilter()
{
	p_num = new ParamInt("number of copies", 2);
	param_layout.push(p_num);
	
	p_menu = new ParamMenu(
			"transform order", 
			std::vector<std::string>{
					"Rot/Scale/Trans",
					"Scale/Rotate/Trans",
					"Scale/Trans/Rotate"});
	param_layout.push(p_menu);
	
	p_translate = new ParamVec3("translate",glm::vec3( 0.0f, 0.0f, 0.0f),"t_");
	param_layout.push(p_translate);
	
	p_rotate = new ParamVec3("rotate",glm::vec3( 0.0f, 0.0f, 0.0f),"r_");
	param_layout.push(p_rotate);	
	
	p_scale = new ParamVec3("scale",glm::vec3( 1.0f, 1.0f, 1.0f),"s_");
	param_layout.push(p_scale);	
}

static void offset_indices(std::vector<unsigned int> & indices, int offset)
{
	for (int i = 0; i < indices.size(); i++)
	{
		indices[i] += offset;
	}
	
}

Mesh DuplicateMeshFilter::applyFilter(Mesh & source_mesh)
{

	Mesh mesh;
	mesh = source_mesh;
	
	
	
	glm::vec3 translate = p_translate->getValue();
	glm::vec3 rotate = p_rotate->getValue();
	glm::vec3 scale = p_scale->getValue();


	Mesh mesh_copy = mesh;
	
	for (int i = 0; i < p_num->getValue(); i++)
	{
		
	
	
		glm::mat4 matrix = glm::mat4(1.0f);
		
		translate += p_translate->getValue();
		rotate += p_rotate->getValue();
		scale += p_scale->getValue();		
		
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
		
		for (int j = 0; j < mesh_copy.vertices.size(); j++)
		{
			mesh_copy.vertices[j].position = matrix * glm::vec4(mesh_copy.vertices[j].position,1.0f);
		}	
		
		mesh.vertices.insert(mesh.vertices.end(), mesh_copy.vertices.begin(), mesh_copy.vertices.end());
		
		offset_indices(mesh_copy.indices, mesh_copy.vertices.size());
		mesh.indices.insert(mesh.indices.end(), mesh_copy.indices.begin(), mesh_copy.indices.end());		
	}		
		

	

	mesh_cache = mesh;
	return mesh;
};


