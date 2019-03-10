#ifndef MESH_UTILS_H
#define MESH_UTILS_H

#include "../pch.h"
#include "../mesh.h"
class MeshUtils
{
	public:
		MeshUtils();
		
		Mesh cylinderMesh(float radius, float height, int segs_height = 5 , int segs_radius = 6);
			
	private:
		/* add your private declarations */
};

#endif /* MESH_UTILS_H */ 
