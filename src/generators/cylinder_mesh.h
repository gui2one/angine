#ifndef CYLINDER_MESH_H
#define CYLINDER_MESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"


class CylinderMesh : public MeshGenerator
{
	public:
		CylinderMesh();
		CylinderMesh(const CylinderMesh& other);
		virtual ~CylinderMesh();
		
		
		
		inline Mesh generate() override{
			Mesh mesh;
			mesh = generateCylinder(
				param_radius->getValue(), 
				param_height->getValue(), 
				param_segs_height->getValue(), 
				param_segs_radius->getValue() 
			);
			
			mesh_cache = mesh;
			
			return mesh;
		};
		Mesh generateCylinder(float radius = 0.5, float height = 1.0, int segs_height = 5, int segs_radius = 20);
		
		//~ json toJSON()override;
		
		ParamFloat* param_radius;
		ParamFloat* param_height;
		ParamInt* param_segs_height;
		ParamInt* param_segs_radius;
		
		
	private:
		/* add your private declarations */
};

#endif /* CYLINDER_MESH_H */ 
