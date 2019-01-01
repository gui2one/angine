#ifndef GEOSPHEREMESH_H
#define GEOSPHEREMESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
class GeoSphereMesh : public MeshGenerator
{
	public:
		GeoSphereMesh();
		virtual ~GeoSphereMesh();
		
		inline Mesh generate() override
		{
			Mesh mesh;
			//~ mesh = generateSphere(paramsInt[0].value, paramsInt[1].value, paramsFloat[0].value, 1.0, 1.0);
			mesh = generateGeoSphere();
			
			mesh_cache = mesh;
			
			return mesh;
		};
		
		Mesh generateSphere(int rows, int cols, float radius = 0.5, float u_ratio = 1.0, float v_ratio = 1.0);
		Mesh generateGeoSphere();

	private:
	
		/* add your private declarations */
};

#endif /* GEOSPHEREMESH_H */ 