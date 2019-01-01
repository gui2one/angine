#ifndef TRANSFORM_MESH_FILTER_H
#define TRANSFORM_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class TransformMeshFilter: public MeshFilter
{
	public:
		TransformMeshFilter();
		
		inline Mesh applyFilter(Mesh & source_mesh)override
		{
			Mesh mesh;
			mesh = source_mesh;
			for (int i = 0; i < mesh.vertices.size(); i++)
			{
				mesh.vertices[i].position.x += paramsFloat[0].value;
				mesh.vertices[i].position.y += paramsFloat[1].value;
				mesh.vertices[i].position.z += paramsFloat[2].value;
			}
			
			glm::mat4 rot_matrix = glm::mat4(1.0f);
			rot_matrix = glm::rotate(rot_matrix, glm::radians( paramsFloat[3].value), glm::vec3(1.0f, 0.0f, 0.0f));
			rot_matrix = glm::rotate(rot_matrix, glm::radians( paramsFloat[4].value), glm::vec3(0.0f, 1.0f, 0.0f));
			rot_matrix = glm::rotate(rot_matrix, glm::radians( paramsFloat[5].value), glm::vec3(0.0f, 0.0f, 1.0f));
			
			for (int i = 0; i < mesh.vertices.size(); i++)
			{
				mesh.vertices[i].position = rot_matrix * glm::vec4(mesh.vertices[i].position,1.0f);
			}
			
			
			//~ std::cout << "hey !!!!\n";
			//~ printf("Applying mesh filter\n");
			mesh_cache = mesh;
			return mesh;
		};
			
	private:
		/* add your private declarations */
};

#endif /* TRANSFORM_MESH_FILTER_H */ 
