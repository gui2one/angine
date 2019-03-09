#include "mirror_mesh_filter.h"
#include <algorithm>
MirrorMeshFilter::MirrorMeshFilter()
	: MeshFilter()
{
	
	p_axis_menu = new ParamMenu("axis", {"X", "Y", "Z"});
	param_layout.push(p_axis_menu);

	
	p_distance = new ParamFloat("distance", 0.0f);
	param_layout.push(p_distance);

}

MirrorMeshFilter::MirrorMeshFilter(const MirrorMeshFilter& other):MeshFilter(other){
	
	p_axis_menu = new ParamMenu(*other.p_axis_menu);
	param_layout.push(p_axis_menu);

	
	p_distance = new ParamFloat(*other.p_distance);
	param_layout.push(p_distance);
}

static void offset_indices(std::vector<unsigned int> & indices, int offset)
{
	for (int i = 0; i < indices.size(); i++)
	{
		indices[i] += offset;
	}
	
}

Mesh MirrorMeshFilter::applyFilter(Mesh & source_mesh)
{

	Mesh mesh;
	mesh = source_mesh;
	
	Mesh mesh_copy = mesh;
	
		glm::mat4 matrix = glm::mat4(1.0f);
		
		if( p_axis_menu->current_choice == 0)
		{
			matrix = glm::translate(matrix, glm::vec3(p_distance->getValue(),0.0f,0.0f) );
			matrix = glm::scale(matrix,glm::vec3(-1.0f , 1.0f, 1.0f));
		}
		else if(p_axis_menu->current_choice == 1)
		{
			matrix = glm::translate(matrix, glm::vec3(0.0f,p_distance->getValue(),0.0f) );
			matrix = glm::scale(matrix,glm::vec3(1.0f , -1.0f, 1.0f));
		}
		else if(p_axis_menu->current_choice == 2)
		{
			matrix = glm::translate(matrix, glm::vec3(0.0f,0.0f,p_distance->getValue()) );
			matrix = glm::scale(matrix,glm::vec3(1.0f ,  1.0f,-1.0f));
		}
		

		

		
		for (int j = 0; j < mesh_copy.vertices.size(); j++)
		{
			mesh_copy.vertices[j].position = matrix * glm::vec4(mesh_copy.vertices[j].position,1.0f);
			mesh_copy.vertices[j].normal = mesh.vertices[j].normal * glm::vec3( -1.0f, 1.0f, 1.0f);
			
		}	
		
		mesh.vertices.insert(mesh.vertices.end(), mesh_copy.vertices.begin(), mesh_copy.vertices.end());
		
		offset_indices(mesh_copy.indices, mesh_copy.vertices.size());
		std::reverse(mesh_copy.indices.begin(), mesh_copy.indices.end());
		mesh.indices.insert(mesh.indices.end(), mesh_copy.indices.begin(), mesh_copy.indices.end());		
			
		

	

	mesh_cache = mesh;
	return mesh;
};


