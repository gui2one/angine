#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "../pch.h"
#include "../mesh.h"
#include "../param.h"
#include <functional>

#include "../param_layout.h"




class MeshGenerator
{
	public:
		MeshGenerator();
		inline virtual ~MeshGenerator(){
			for (size_t i = 0; i < param_layout.getSize(); i++)
			{
				delete param_layout.getParam(i);
			}
			
			printf("Just delete paramLayout content\n");			
		}
		inline virtual Mesh generate(){}

		bool need_update = false;
		
		Mesh mesh_cache;
		
		ParamLayout param_layout;

		
		
	private:
		/* add your private declarations */
};

#endif /* MESH_GENERATOR_H */ 
