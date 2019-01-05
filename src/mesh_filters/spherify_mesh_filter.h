#ifndef SPHERIFY_MESH_FILTER_H
#define SPHERIFY_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class SpherifyMeshFilter: public MeshFilter
{
	public:
		SpherifyMeshFilter();
		
		inline Mesh applyFilter(Mesh & source_mesh)override
		{
			Mesh mesh;
			mesh = source_mesh;

			for (int i = 0; i < mesh.vertices.size() ; i++)
			{
				mesh.vertices[i].position = glm::normalize(mesh.vertices[i].position) * paramsFloat[0].value;
			}
			
			mesh_cache = mesh;
			return mesh;
		};
			
	private:
		std::string name = "spherify";
		/* add your private declarations */
};

#endif /* SPHERIFY_MESH_FILTER_H */ 
