#include "mesh_generator.h"


MeshGenerator::MeshGenerator()
{
	
}

MeshGenerator::MeshGenerator(const MeshGenerator& other){
	//~ printf("MeshGenerator COPY CONSTRUCTOR\n");
	mesh_cache = other.mesh_cache;
		
	//~ param_layout = other.param_layout;	
}

MeshGenerator::~MeshGenerator(){
	for (size_t i = 0; i < param_layout.getSize(); i++)
	{
		delete param_layout.getParam(i);
	}
	
	//~ printf("Just delete paramLayout content\n");			
}


