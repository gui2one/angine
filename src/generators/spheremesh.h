#ifndef SPHEREMESH_H
#define SPHEREMESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
class SphereMesh : public MeshGenerator
{
	public:
		SphereMesh();
		SphereMesh(const SphereMesh& other);
		virtual ~SphereMesh();
		
		inline Mesh generate() override
		{
			Mesh mesh;
			mesh = generateSphere(p_rows->getValue(), p_cols->getValue(), p_radius->getValue(), p_u_ratio->getValue(), p_v_ratio->getValue());
			
			mesh_cache = mesh;
			
			return mesh;
		};
		
		Mesh generateSphere(int rows, int cols, float radius = 0.5, float u_ratio = 1.0, float v_ratio = 1.0);
		


		ParamInt* p_rows;
		ParamInt* p_cols;
		
		ParamFloat* p_radius;
		ParamFloat* p_u_ratio;
		ParamFloat* p_v_ratio;
	private:
	
		/* add your private declarations */
};

#endif /* SPHEREMESH_H */ 
