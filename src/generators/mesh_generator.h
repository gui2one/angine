#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "../pch.h"
#include "../mesh.h"
#include <functional>
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
		std::vector<Param<std::string>> paramsString;
		std::vector<Param<void (*)()>> paramsAction;
		inline virtual Mesh generate(){}; 
		
	private:
		/* add your private declarations */
};

#endif /* MESH_GENERATOR_H */ 
