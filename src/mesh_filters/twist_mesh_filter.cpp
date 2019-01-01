#include "twist_mesh_filter.h"


TwistMeshFilter::TwistMeshFilter()
	: MeshFilter()
{
	Param<float> amount{"amount",0.0};
	paramsFloat.push_back(amount);
	
	Param<int> axis_choice{"axis choice",0};
	paramsInt.push_back(axis_choice);	
	
	Param<glm::vec3> axis{"axis",glm::vec3(0.0f,0.0f, 1.0f)};
	paramsVec3.push_back(axis);
		
}

Mesh TwistMeshFilter::applyFilter(Mesh & source_mesh)
{
	Mesh mesh;
	mesh = source_mesh;

	if(paramsInt[0].value == 0){
		
		glm::mat4 rot_matrix = glm::mat4(1.0f);
		
		
		for (int i = 0; i < mesh.vertices.size(); i++)
		{
			glm::mat4 temp_matrix = glm::mat4(1.0f);
			temp_matrix = glm::rotate(rot_matrix, mesh.vertices[i].position.z * 2.0f, glm::vec3( 0.0f, 0.0f, 1.0f));
			
			mesh.vertices[i].position = temp_matrix * glm::vec4(mesh.vertices[i].position,1.0f);
		}
		
	}
	
	mesh_cache = mesh;
	return mesh;
};


