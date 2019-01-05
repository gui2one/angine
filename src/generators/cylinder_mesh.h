#ifndef CYLINDER_MESH_H
#define CYLINDER_MESH_H

#include "../pch.h"
#include "../mesh.h"
#include "mesh_generator.h"
class CylinderMesh : public MeshGenerator
{
	public:
		CylinderMesh();
		virtual ~CylinderMesh();
		
		inline Mesh generate() override{
			Mesh mesh;
			mesh = generateCylinder(paramsFloat[0].value, paramsFloat[1].value, paramsInt[0].value, paramsInt[1].value );
			
			mesh_cache = mesh;
			
			return mesh;
		};
		Mesh generateCylinder(float radius = 0.5, float height = 1.0, int segs_height = 5, int segs_radius = 20);
		
	private:
		/* add your private declarations */
};

#endif /* CYLINDER_MESH_H */ 
