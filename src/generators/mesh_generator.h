#ifndef MESH_GENERATOR_H
#define MESH_GENERATOR_H

#include "../pch.h"
#include "../mesh.h"

#include <functional>

#include "../param_layout.h"


enum MESH_GENERATOR_TYPE{
	SPHERE_MESH_GENERATOR,
	GEOSPHERE_MESH_GENERATOR,
	GRID_MESH_GENERATOR,
	BOX_MESH_GENERATOR,
	CYLINDER_MESH_GENERATOR,
	FILE_MESH_GENERATOR
	
};

class MeshGenerator
{
	public:
		MeshGenerator();
		MeshGenerator(const MeshGenerator& other);
		virtual ~MeshGenerator();
		
		
		
		inline virtual Mesh generate(){}
		virtual json toJSON();
		bool need_update = false;
		
		Mesh mesh_cache;
		
		ParamLayout param_layout;

		inline void setType(MESH_GENERATOR_TYPE _type){ type = _type;}
		inline MESH_GENERATOR_TYPE getType() const { return type;}
		
		
	private:
	
		MESH_GENERATOR_TYPE type;
		/* add your private declarations */
};

#endif /* MESH_GENERATOR_H */ 
