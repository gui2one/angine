#ifndef INFLATE_MESH_FILTER_H
#define INFLATE_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class InflateMeshFilter: public MeshFilter
{
	public:
		InflateMeshFilter();
		
		inline Mesh applyFilter(Mesh & source_mesh)override
		{
			Mesh mesh;
			mesh = source_mesh;
			for (int i = 0; i < mesh.vertices.size(); i++)
			{
				
				mesh.vertices[i].position.x += mesh.vertices[i].normal.x * paramsFloat[0].value;
				mesh.vertices[i].position.y += mesh.vertices[i].normal.y * paramsFloat[0].value;
				mesh.vertices[i].position.z += mesh.vertices[i].normal.z * paramsFloat[0].value;
			}
			
			//~ std::cout << "hey !!!!\n";
			//~ printf("Applying mesh filter\n");
			mesh_cache = mesh;
			return mesh;
		};
			
	private:
		/* add your private declarations */
};

#endif /* INFLATE_MESH_FILTER_H */ 
