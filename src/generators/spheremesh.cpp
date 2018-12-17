#include "spheremesh.h"

#define PI 3.14159265359
SphereMesh::SphereMesh()
{
	
}


SphereMesh::~SphereMesh()
{
	
}

void SphereMesh::generate(Mesh& mesh, int rows, int cols, float radius /*= 0.5*/, float u_ratio, float v_ratio){
		
		
		std::vector<Vertex> vertices;
		
		for (int j = 0; j < rows-1; j++)
		{
			
			for (int i = 0; i < cols-1; i++)
			{

				// start at (0, radius , O)
				
				
				Vertex vert;
				float ratio_cols = 1.0/((float)cols-1) * (float)i * u_ratio;
				float ratio_rows = 1.0/((float)rows-1) * (float)j * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0 ) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0 ) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);

				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(ratio_cols,ratio_rows);			
				
				vertices.push_back(vert);		


				ratio_cols = 1.0/((float)cols-1) * ((float)i+1) * u_ratio;
				ratio_rows = 1.0/((float)rows-1) * ((float)j+1) * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(ratio_cols,ratio_rows);	
				
				vertices.push_back(vert);					
				
				
				ratio_cols = 1.0/((float)cols-1) * (float)i * u_ratio;
				ratio_rows = 1.0/((float)rows-1) * ((float)j+1) * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(ratio_cols,ratio_rows);	
								
				vertices.push_back(vert);	
				
				
					

				
				
				
				
				//// second triangle	
				
				ratio_cols = 1.0/((float)cols-1) * (float)i * u_ratio;
				ratio_rows = 1.0/((float)rows-1) * (float)j * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);

				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(ratio_cols,ratio_rows);			
				
				vertices.push_back(vert);		


				ratio_cols = 1.0/((float)cols-1) * ((float)i+1) * u_ratio;
				ratio_rows = 1.0/((float)rows-1) * ((float)j) * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(ratio_cols,ratio_rows);	
				
				vertices.push_back(vert);
				
								
				ratio_cols = 1.0/((float)cols-1) * ((float)i+1) * u_ratio;
				ratio_rows = 1.0/((float)rows-1) * ((float)j+1) * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.y = cos( ratio_cols * PI *2.0) * sin(ratio_rows * PI);
				vert.position.z = cos(ratio_rows * PI);
				
				vert.normal = glm::normalize(vert.position);
				vert.t_coords = glm::vec2(ratio_cols,ratio_rows);	
								
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


Mesh SphereMesh::generate2(int rows, int cols, float radius, float u_ratio, float v_ratio)
{
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	indices.clear();
	vertices.clear();
	
	Vertex vert;
	//// top vertex
	vert.position.x = 0.0;
	vert.position.y = 0.0;
	vert.position.z = radius;	

	vert.normal.x = 0.0;
	vert.normal.y = 0.0;
	vert.normal.z = 1.0;	

	vert.t_coords.x = 0.5;
	vert.t_coords.y = 0.0;	
		
	vertices.push_back(vert);
	
	
	for (int i = 1; i < rows; i++)
	{
		//~ std::cout << "Row " << i << "\n";
		for (int j = 0; j < cols+1; j++)
		{
				float ratio_cols = 1.0/((float)cols) * (float)j * u_ratio;
				float ratio_rows = 1.0/((float)rows-1) * (float)i * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0 ) * sin(ratio_rows * PI) * radius;
				vert.position.y = cos( ratio_cols * PI *2.0 ) * sin(ratio_rows * PI) * radius;
				vert.position.z = cos(ratio_rows * PI) * radius;
				
				//~ std::cout << "col : " << j << " --> ";
				//~ std::cout << vert.position.x;
				//~ std::cout << " , " << vert.position.y;
				//~ std::cout << " , " << vert.position.z << "\n";
				
				vert.normal = glm::normalize(vert.position);	

				vert.t_coords.x = ratio_rows;
				vert.t_coords.y = ratio_cols;					
				
				vertices.push_back(vert);			
		}
		
	}
	
	
	//// bottom vertex
	vert.position.x = 0.0;
	vert.position.y = 0.0;
	vert.position.z = -radius;	
	
	vert.normal.x = 0.0;
	vert.normal.y = 0.0;
	vert.normal.z = 1.0;	

	vert.t_coords.x = 0.5;
	vert.t_coords.y = 1.0;	
	
	vertices.push_back(vert);	
	
	
	//// build indices

	//// first row of triangles
	for (int i = 0; i < cols; i++)
	{
		indices.push_back(0);
		indices.push_back(i+1);

			indices.push_back(i+2);
		
	}
	
	//// build inside quads ( 2 triangles)
	for (int i = 0; i < rows-3; i++)
	{
		for (int j = 0; j < cols+1; j++)
		{
			int first_in_row = (i * (cols + 1));
			indices.push_back(first_in_row + j);
			indices.push_back(first_in_row + j + cols+1);
			indices.push_back(first_in_row + j + 1);		
			
			indices.push_back(first_in_row + j + 1);
			indices.push_back(first_in_row + j + cols+1);
			indices.push_back(first_in_row + j + cols+2);
							
		}		
	}
	
	
	//// last row of triangles
	for (int i = 0; i < cols; i++)
	{		
		indices.push_back((rows-2) * (cols+1) - (cols) + i);
		indices.push_back(vertices.size()-1);
		indices.push_back((rows-2) * (cols+1) - (cols) + i + 1);
		
	}	
	
	
	Mesh mesh;
	
	mesh.vertices = vertices;
	mesh.indices = indices;
	
	return mesh;
} 
