#ifndef GRIDMESH_H
#define GRIDMESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
class GridMesh : public MeshGenerator
{
	public:
		GridMesh();
		GridMesh(const GridMesh& other);
		virtual ~GridMesh();
		
		inline Mesh generate() override{
			Mesh mesh;
			mesh = generateGrid(p_rows->getValue(), p_cols->getValue(), p_width->getValue(), p_length->getValue() );
			
			mesh_cache = mesh;
			
			return mesh;
		};
		Mesh generateGrid(int rows, int cols=2, float width= 1.0, float length = 1.0);
		
		
		ParamInt *p_rows;
		ParamInt *p_cols;
		
		ParamFloat *p_width;
		ParamFloat *p_length;
	private:
		/* add your private declarations */
};

#endif /* GRIDMESH_H */ 
