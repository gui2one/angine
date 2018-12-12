#ifndef GRIDMESH_H
#define GRIDMESH_H

#include "../pch.h"
#include "../mesh.h"
class GridMesh
{
	public:
		GridMesh();
		virtual ~GridMesh();
		Mesh generate(int rows, int cols, float width= 1.0, float length = 1.0);
	private:
		/* add your private declarations */
};

#endif /* GRIDMESH_H */ 
