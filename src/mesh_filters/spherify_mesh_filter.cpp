#include "spherify_mesh_filter.h"


SpherifyMeshFilter::SpherifyMeshFilter()
	: MeshFilter()
{
	p_radius = new ParamFloat("radius", 0.5);
	param_layout.push(p_radius);
}

SpherifyMeshFilter::SpherifyMeshFilter(const SpherifyMeshFilter& other):MeshFilter(other){
	p_radius = new ParamFloat(*other.p_radius);
	param_layout.push(p_radius);	
}

Mesh SpherifyMeshFilter::applyFilter(Mesh & source_mesh)
{
	Mesh mesh;
	mesh = source_mesh;

	for (int i = 0; i < mesh.vertices.size() ; i++)
	{
		mesh.vertices[i].position = glm::normalize(mesh.vertices[i].position) * p_radius->getValue();
	}
	
	mesh_cache = mesh;
	return mesh;
};


