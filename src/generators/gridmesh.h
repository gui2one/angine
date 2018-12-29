#ifndef GRIDMESH_H
#define GRIDMESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
class GridMesh : public MeshGenerator
{
	public:
		GridMesh();
		virtual ~GridMesh();
		
		inline Mesh generate() override{
			Mesh mesh;
			mesh = generateGrid(paramsInt[0].value, paramsInt[1].value, paramsFloat[0].value, paramsFloat[1].value );
			
			return mesh;
		};
		Mesh generateGrid(int rows, int cols=2, float width= 1.0, float length = 1.0);
		
	private:
		/* add your private declarations */
};

#endif /* GRIDMESH_H */ 
