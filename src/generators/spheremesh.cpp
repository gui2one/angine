#include "spheremesh.h"

#define PI 3.14159265359
SphereMesh::SphereMesh()
{
	Param<int> param1{"cols", 42};
	paramsInt.push_back(param1);	

	//~ param_cols = param1.value;
	
	Param<int> param2{"rows", 12};
	paramsInt.push_back(param2);	
	
	//~ param_rows = param2.value;
	
	Param<float> param3{"radius", 1.0};
	paramsFloat.push_back(param3);		
}


SphereMesh::~SphereMesh()
{
	
}



Mesh SphereMesh::generateSphere(int rows, int cols, float radius, float u_ratio, float v_ratio)
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
		
		for (int j = 0; j < cols+1; j++)
		{
				float ratio_cols = 1.0/((float)cols) * (float)j * u_ratio;
				float ratio_rows = 1.0/((float)rows-1) * (float)i * v_ratio;
				vert.position.x = sin( ratio_cols * PI *2.0 ) * sin(ratio_rows * PI) * radius;
				vert.position.y = cos( ratio_cols * PI *2.0 ) * sin(ratio_rows * PI) * radius;
				vert.position.z = cos(ratio_rows * PI) * radius;
				
				
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
	
	
	indices.clear();
	vertices.clear();
	return mesh;
} 
