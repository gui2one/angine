#include "twist_mesh_filter.h"


TwistMeshFilter::TwistMeshFilter()
	: MeshFilter()
{

	setType(TWIST_MESH_FILTER);
	
	p_amount = new ParamFloat("amount", 0.1);
	param_layout.push(p_amount);
	
	p_axis_choice = new ParamInt("axis choice", 0);
	param_layout.push(p_axis_choice);	
}

TwistMeshFilter::TwistMeshFilter(const TwistMeshFilter& other): MeshFilter(other){
	
	p_amount = new ParamFloat(*other.p_amount);
	param_layout.push(p_amount);
	
	p_axis_choice = new ParamInt(*other.p_axis_choice);
	param_layout.push(p_axis_choice);		
}

Mesh TwistMeshFilter::applyFilter(Mesh & source_mesh)
{
	Mesh mesh;
	mesh = source_mesh;
	glm::vec3 axis = glm::vec3(1.0f);
	float ratio = 0.0f;
	if(p_axis_choice->getValue() == 0){			
		axis = glm::vec3(1.0f, 0.0f, 0.0f);		
		
	}else if(p_axis_choice->getValue() == 1){		
		axis = glm::vec3(0.0f, 1.0f, 0.0f);		
		
	}else if(p_axis_choice->getValue() == 2){
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
		
	}
	
	
	glm::mat4 rot_matrix = glm::mat4(1.0f);
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		
		glm::mat4 temp_matrix = glm::mat4(1.0f);
		
		if(p_axis_choice->getValue() == 0){
			ratio = mesh.vertices[i].position.x;
		}else if(p_axis_choice->getValue() == 1){			
			ratio = mesh.vertices[i].position.y;
		}else if(p_axis_choice->getValue() == 2){			
			ratio = mesh.vertices[i].position.z;		
		}
		temp_matrix = glm::rotate(rot_matrix, ratio * p_amount->getValue(), axis);
		
		mesh.vertices[i].position = temp_matrix * glm::vec4(mesh.vertices[i].position,1.0f);
	}
	
	
	
	mesh_cache = mesh;
	return mesh;
};


