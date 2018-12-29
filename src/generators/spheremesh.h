#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
class SphereMesh : public MeshGenerator
{
	public:
		SphereMesh();
		virtual ~SphereMesh();
		
		inline Mesh generate() override
		{
			Mesh mesh;
			mesh = generateSphere(paramsInt[0].value, paramsInt[1].value, paramsFloat[0].value, 1.0, 1.0);
			
			return mesh;
		};
		
		Mesh generateSphere(int rows, int cols, float radius = 0.5, float u_ratio = 1.0, float v_ratio = 1.0);
		

	private:
	
		/* add your private declarations */
};

#endif /* SPHEREMESH_H */ 
