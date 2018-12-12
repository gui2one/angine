#include "spheremesh.h"

#define PI 3.14159265359
SphereMesh::SphereMesh()
{
	
}


SphereMesh::~SphereMesh()
{
	
}

void SphereMesh::generate(Mesh& mesh, int rows, int cols, float radius /*= 0.5*/){
		
		
		std::vector<Vertex> vertices;
		
		for (int j = 0; j < rows-1; j++)
		{
			
			for (int i = 0; i < cols-1; i++)
			{

				// start at (0, radius , O)
				
				
				Vertex vert;
				float ratio_cols = 1.0/((float)cols-1) * (float)i;
				float ratio_rows = 1.0/((float)rows-1) * (float)j;
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);

				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(0.0,1.0);			
				
				vertices.push_back(vert);		
				
				ratio_cols = 1.0/((float)cols-1) * (float)i;
				ratio_rows = 1.0/((float)rows-1) * ((float)j+1);
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(0.0,1.0);	
								
				vertices.push_back(vert);	
					
				ratio_cols = 1.0/((float)cols-1) * ((float)i+1);
				ratio_rows = 1.0/((float)rows-1) * ((float)j+1);
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(0.0,1.0);	
				
				vertices.push_back(vert);	
				
				
				
				
				//// second triangle	
				
				ratio_cols = 1.0/((float)cols-1) * (float)i;
				ratio_rows = 1.0/((float)rows-1) * (float)j;
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);

				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(0.0,1.0);			
				
				vertices.push_back(vert);		
				
				ratio_cols = 1.0/((float)cols-1) * ((float)i+1);
				ratio_rows = 1.0/((float)rows-1) * ((float)j+1);
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(0.0,1.0);	
								
				vertices.push_back(vert);	
					
				ratio_cols = 1.0/((float)cols-1) * ((float)i+1);
				ratio_rows = 1.0/((float)rows-1) * ((float)j);
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(0.0,1.0);	
				
				vertices.push_back(vert);												
				
				//~ std::cout <<  "row ratio : " << sin(ratio_rows * PI) << "\n";
				//~ std::cout <<  "Z pos : " << cos(ratio_rows * PI) << "\n";
				//~ std::cout << "vertex : \n\tX : " <<  vert.position.x;
				//~ std::cout << " , Y : " <<  vert.position.y;
				//~ std::cout << " , Z : " <<  vert.position.z << "\n";
			}
			
		}
		
		mesh.vertices = vertices;
		vertices.clear();

}

