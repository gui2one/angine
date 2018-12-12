#ifndef MESH_H
#define MESH_H


#include <iostream>
#include <vector>

#include "vertex.h"
//~ #include "vector3.h"


class Mesh{
	public :
		Mesh();
		~Mesh();
		
		void clearAll();
		std::vector<Vertex> vertices;
		std::vector<float> positions; // 3 floats per vertex
		std::vector<float> normals; // 3 floats per vertex
		std::vector<float> t_coords; // 2 floats per vertex
		std::vector<int> indices;
};

#endif
