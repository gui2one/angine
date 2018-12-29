#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "../pch.h"
#include "../mesh.h"
//~ #include "spheremesh.h"
//~ #include "gridmesh.h"

template<typename T>
struct Param{
		
	std::string name;
	T value;
};

class MeshGenerator
{
	public:
		MeshGenerator();
		
		
		std::vector<Param<int>> paramsInt;
		std::vector<Param<float>> paramsFloat;
		inline virtual Mesh generate(){}; 
		inline virtual Mesh generate(int rows, int cols, float radius = 0.5, float u_ratio = 1.0, float v_ratio = 1.0){}; 
	private:
		/* add your private declarations */
};

#endif /* MESH_GENERATOR_H */ 
