#ifndef GEOSPHEREMESH_H
#define GEOSPHEREMESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
class GeoSphereMesh : public MeshGenerator
{
	public:
		GeoSphereMesh();
		GeoSphereMesh(const GeoSphereMesh& other);
		virtual ~GeoSphereMesh();
		
		inline Mesh generate() override
		{
			Mesh mesh;
			
			mesh = generateGeoSphere(p_radius->getValue());
			
			mesh_cache = mesh;
			
			return mesh;
		};
		
		
		Mesh generateGeoSphere(float radius);


		
		ParamFloat* p_radius;
		
	private:
	
		/* add your private declarations */
};

#endif /* GEOSPHEREMESH_H */ 
