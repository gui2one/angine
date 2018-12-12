#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "../pch.h"
#include "../mesh.h"
class SphereMesh
{
	public:
		SphereMesh();
		virtual ~SphereMesh();
		
		void generate(Mesh& mesh, int rows=5, int cols=5, float radius = 0.5);
	private:
		/* add your private declarations */
};

#endif /* SPHEREMESH_H */ 
