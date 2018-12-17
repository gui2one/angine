#ifndef MESH_H
#define MESH_H


#include <iostream>
#include <vector>

#include "vertex.h"
//~ #include "vector3.h"


class Mesh{
	public :
		Mesh();
		//~ ~Mesh();
		
		void clearAll();
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
};

#endif
