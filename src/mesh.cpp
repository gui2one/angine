#include "mesh.h"

Mesh::Mesh(){
	std::cout << "Mesh instance Created" << std::endl;
}


void Mesh::clearAll(){
	vertices.clear();
	positions.clear(); // 3 floats per vertex
	normals.clear(); // 3 floats per vertex
	t_coords.clear(); // 2 floats per vertex
	indices.clear();
}

Mesh::~Mesh(){
		vertices.clear();
		positions.clear(); // 3 floats per vertex
		normals.clear(); // 3 floats per vertex
		t_coords.clear(); // 2 floats per vertex
		indices.clear();
	std::cout << "--- Mesh instance Deleted ----" << std::endl;
}
