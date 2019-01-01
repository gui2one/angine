#include "compute_normals_mesh_filter.h"


ComputeNormalsMeshFilter::ComputeNormalsMeshFilter()
	: MeshFilter()
{
	Param<float> param1{"tx",0.0};
	paramsFloat.push_back(param1);	
}

Mesh ComputeNormalsMeshFilter::applyFilter(Mesh & source_mesh)
{
			Mesh mesh;
			mesh = source_mesh;
			
			std::vector<int> checked_indices;
			
			std::vector<int> num_normals(mesh.vertices.size());
			for (int i = 0; i < num_normals.size(); i++)
			{
				num_normals[i] = 0;
			}
			
			std::vector<glm::vec3> temp_normals(mesh.vertices.size());
			for (int i = 0; i < temp_normals.size(); i++)
			{
				temp_normals[i] = glm::vec3(0.0f,0.0f,0.0f);
			}			
			// compute normals

			for (int i = 0; i < mesh.indices.size(); i++)
			{
				glm::vec3 AB;
				glm::vec3 AC;
				
				if((i % 3) == 0){					
					AB = mesh.vertices[mesh.indices[i+1]].position - mesh.vertices[mesh.indices[i]].position;
					AC = mesh.vertices[mesh.indices[i+2]].position - mesh.vertices[mesh.indices[i]].position;					
				}else if((i % 3) == 1){					
					AB = mesh.vertices[mesh.indices[i+1]].position - mesh.vertices[mesh.indices[i]].position;
					AC = mesh.vertices[mesh.indices[i-1]].position - mesh.vertices[mesh.indices[i]].position;										
				}else if((i % 3) == 2){					
					AB = mesh.vertices[mesh.indices[i-2]].position - mesh.vertices[mesh.indices[i]].position;
					AC = mesh.vertices[mesh.indices[i-1]].position - mesh.vertices[mesh.indices[i]].position;										
				}
					
				num_normals[mesh.indices[i]] += 1;
				
				glm::vec3 n_normal = glm::normalize(glm::cross(glm::normalize(AB),glm::normalize(AC)));
				
				temp_normals[mesh.indices[i]] += n_normal;

			}
			
			for (int i = 0; i < mesh.vertices.size(); i++)
			{
				//~ printf("Vertex %d : \n", i);
				//~ printf("\tnum normals %d : \n", num_normals[i]);
				//~ printf("\taccu normal %.3f, %.3f, %.3f : \n", temp_normals[i].x, temp_normals[i].y, temp_normals[i].z);
				mesh.vertices[i].normal = glm::normalize(temp_normals[i] / (float)(num_normals[i]));
			}
			
			
			


			////
			mesh_cache = mesh;
			return mesh;
		
}


