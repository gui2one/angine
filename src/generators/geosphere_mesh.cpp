#include "geosphere_mesh.h"

#define PI 3.14159265359
GeoSphereMesh::GeoSphereMesh()
{	

	
	p_radius = new ParamFloat("radius", 0.5);
	param_layout.push(p_radius);
}


GeoSphereMesh::~GeoSphereMesh()
{
	
}



static void add_triangle(std::vector<unsigned int>& _indices,unsigned int id1,unsigned int id2,unsigned int id3){
	_indices.push_back(id1);
	_indices.push_back(id2);
	_indices.push_back(id3);
}


Mesh GeoSphereMesh::generateGeoSphere(float radius)
{
	
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	// create 12 vertices of a icosahedron
	float t = (1.0 + sqrt(5.0)) / 2.0;
	float scale_down_ratio = (1.0 /  t);
	
	glm::vec3 pos;
	
	pos = {-1.0 * scale_down_ratio,  t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius, glm::normalize(pos)});
	pos = {1.0 * scale_down_ratio,  t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius,glm::normalize(pos)});
	pos = {-1.0 * scale_down_ratio,  -t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius, glm::normalize(pos)});
	pos = {1.0 * scale_down_ratio,  -t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius,glm::normalize(pos)});
	
	pos = { 0.0,-1.0 * scale_down_ratio, t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius ,glm::normalize(pos)});
	pos = { 0.0, 1.0 * scale_down_ratio, t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius,glm::normalize(pos)});
	pos = { 0.0,-1.0 * scale_down_ratio,-t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius,glm::normalize(pos)});
	pos = { 0.0, 1.0 * scale_down_ratio,-t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius,glm::normalize(pos)});
	
	pos = { t * scale_down_ratio, 0.0,-1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius, glm::normalize(pos)});
	pos = { t * scale_down_ratio, 0.0, 1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius,glm::normalize(pos)});
	pos = {-t * scale_down_ratio, 0.0,-1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius,glm::normalize(pos)});
	pos = {-t * scale_down_ratio, 0.0, 1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) * radius ,glm::normalize(pos)});



//~ // 5 faces around point 0
	add_triangle(indices,0,11, 5);
	add_triangle(indices,0, 5, 1);
	add_triangle(indices,0, 1, 7);
	add_triangle(indices,0, 7,10);
	add_triangle(indices,0,10,11);

//~ 
//~ // 5 adjacent faces
	add_triangle(indices, 1, 5, 9);
	add_triangle(indices, 5,11, 4);
	add_triangle(indices,11,10, 2);
	add_triangle(indices,10, 7, 6);
	add_triangle(indices, 7, 1, 8);

//~ 
//~ // 5 faces around point 3
	add_triangle(indices, 3, 9, 4);
	add_triangle(indices, 3, 4, 2);
	add_triangle(indices, 3, 2, 6);
	add_triangle(indices, 3, 6, 8);
	add_triangle(indices, 3, 8, 9);

//~ 
//~ // 5 adjacent faces
	add_triangle(indices, 4, 9, 5);
	add_triangle(indices, 2, 4,11);
	add_triangle(indices, 6, 2,10);
	add_triangle(indices, 8, 6, 7);
	add_triangle(indices, 9, 8, 1);

	
	Mesh mesh;
	mesh.vertices = vertices;
	mesh.indices = indices;
	//~ vertices.clear();
	return mesh;
}
