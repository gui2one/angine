#include "mesh.h"

Mesh::Mesh(){
	//~ std::cout << "Mesh instance Created\n";
}



void Mesh::computeNormals(){
			//~ Mesh mesh;
			//~ mesh = source_mesh;
			
			std::vector<int> checked_indices;
			
			std::vector<int> num_normals(vertices.size());
			for (int i = 0; i < num_normals.size(); i++)
			{
				num_normals[i] = 0;
			}
			
			std::vector<glm::vec3> temp_normals(vertices.size());
			for (int i = 0; i < temp_normals.size(); i++)
			{
				temp_normals[i] = glm::vec3(0.0f,0.0f,0.0f);
			}			
			// compute normals

			for (int i = 0; i < indices.size(); i++)
			{
				glm::vec3 AB;
				glm::vec3 AC;
				
				if((i % 3) == 0){					
					AB = vertices[indices[i+1]].position - vertices[indices[i]].position;
					AC = vertices[indices[i+2]].position - vertices[indices[i]].position;					
				}else if((i % 3) == 1){					
					AB = vertices[indices[i+1]].position - vertices[indices[i]].position;
					AC = vertices[indices[i-1]].position - vertices[indices[i]].position;										
				}else if((i % 3) == 2){					
					AB = vertices[indices[i-2]].position - vertices[indices[i]].position;
					AC = vertices[indices[i-1]].position - vertices[indices[i]].position;										
				}
					
				num_normals[indices[i]] += 1;
				
				glm::vec3 n_normal = glm::cross(glm::normalize(AB),glm::normalize(AC));
				
				temp_normals[indices[i]] += n_normal;

			}
			
			for (int i = 0; i < vertices.size(); i++)
			{
			
				vertices[i].normal = glm::normalize(temp_normals[i]);

			}
			
			
			


			////
			//~ mesh_cache = mesh;
			//~ return mesh;	
}
void Mesh::clearAll(){
	vertices.clear();
	indices.clear();
}

//~ Mesh::~Mesh(){
	//~ vertices.clear();		
	//~ indices.clear();
	//~ std::cout << "--- Mesh instance Deleted ----\n";
//~ }
