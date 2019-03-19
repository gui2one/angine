#include "inflate_mesh_filter.h"


InflateMeshFilter::InflateMeshFilter()
	: MeshFilter()
{
	setType(INFLATE_MESH_FILTER);
	
	p_amount = new ParamFloat("amount",0.0);
	param_layout.push(p_amount);

}

InflateMeshFilter::InflateMeshFilter(const InflateMeshFilter& other): MeshFilter(other){
	p_amount = new ParamFloat(*other.p_amount);
	param_layout.push(p_amount);	
}

Mesh InflateMeshFilter::applyFilter(Mesh & source_mesh)
{
	Mesh mesh;
	mesh = source_mesh;
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		mesh.vertices[i].position += mesh.vertices[i].normal * p_amount->getValue();
	}
	

	mesh_cache = mesh;
	return mesh;
};


