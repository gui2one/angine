#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "../pch.h"
#include "../mesh.h"
class SphereMesh
{
	public:
		SphereMesh();
		virtual ~SphereMesh();
		
		void generate(Mesh& mesh, int rows=5, int cols=5, float radius = 0.5, float u_ratio = 1.0, float v_ratio = 0.6);
		Mesh generate2(int rows=5, int cols=5, float radius = 0.5, float u_ratio = 1.0, float v_ratio = 1.0);
	private:
		/* add your private declarations */
};

#endif /* SPHEREMESH_H */ 
