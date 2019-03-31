#ifndef MESH_UTILS_H
#define MESH_UTILS_H

#include "../pch.h"
#include "../mesh.h"
class MeshUtils
{
	public:
		MeshUtils();
		
		Mesh cylinderMesh(float radius, float height, int segs_height = 2 , int segs_radius = 6);
		Mesh coneMesh(float radius1, float radius2, float height, int segs_height = 2 , int segs_radius = 6);
			
		Mesh merge(Mesh& mesh1, Mesh& mesh2);
	private:
		/* add your private declarations */
};

#endif /* MESH_UTILS_H */ 
