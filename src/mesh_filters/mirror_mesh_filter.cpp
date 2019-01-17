#include "mirror_mesh_filter.h"


MirrorMeshFilter::MirrorMeshFilter()
	: MeshFilter()
{
	p_center_pos = new ParamVec3("center", glm::vec3(0.0f,0.0f, 0.0f));
	param_layout.push(p_center_pos);
	
	p_distance = new ParamFloat("distance", 1.0f);
	param_layout.push(p_distance);
	
	p_axis = new ParamVec3("axis", glm::vec3(0.0f,0.0f, 1.0f));
	param_layout.push(p_axis);
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
		
	
		
		
		matrix = glm::translate(matrix, glm::vec3(p_distance->getValue(),0.0f,0.0f) );
		matrix = glm::scale(matrix,glm::vec3(-1.0f,1.0f,1.0f));
		for (int j = 0; j < mesh_copy.vertices.size(); j++)
		{
			mesh_copy.vertices[j].position = matrix * glm::vec4(mesh_copy.vertices[j].position,1.0f);
		}	
		
		mesh.vertices.insert(mesh.vertices.end(), mesh_copy.vertices.begin(), mesh_copy.vertices.end());
		
		offset_indices(mesh_copy.indices, mesh_copy.vertices.size());
		mesh.indices.insert(mesh.indices.end(), mesh_copy.indices.begin(), mesh_copy.indices.end());		
			
		

	

	mesh_cache = mesh;
	return mesh;
};


