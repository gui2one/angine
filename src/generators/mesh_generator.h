#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "../pch.h"
#include "../mesh.h"
#include "../param.h"
#include <functional>
//~ #include "spheremesh.h"
//~ #include "gridmesh.h"

//~ template<typename T>
//~ struct Param{
		//~ 
	//~ std::string name;
	//~ T value;
//~ };

class MeshGenerator
{
	public:
		MeshGenerator();
		inline virtual Mesh generate(){}; 
		
		bool need_update = false;
		
		Mesh mesh_cache;
		
		std::vector<Param<int>> paramsInt;
		std::vector<Param<float>> paramsFloat;
		std::vector<Param<std::string>> paramsString;
		std::vector<Param<std::function<void()>>> paramsAction;
		
		
	private:
		/* add your private declarations */
};

#endif /* MESH_GENERATOR_H */ 
