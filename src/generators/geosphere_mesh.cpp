#include "geosphere_mesh.h"

#define PI 3.14159265359
GeoSphereMesh::GeoSphereMesh()
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


GeoSphereMesh::~GeoSphereMesh()
{
	
}



Mesh GeoSphereMesh::generateSphere(int rows, int cols, float radius, float u_ratio, float v_ratio)
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


static void add_triangle(std::vector<unsigned int>& _indices,unsigned int id1,unsigned int id2,unsigned int id3){
	_indices.push_back(id1);
	_indices.push_back(id2);
	_indices.push_back(id3);
}


Mesh GeoSphereMesh::generateGeoSphere()
{
	
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	// create 12 vertices of a icosahedron
	float t = (1.0 + sqrt(5.0)) / 2.0;
	float scale_down_ratio = 1.0 /  t;
	
	glm::vec3 pos;
	
	pos = {-1.0 * scale_down_ratio,  t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos), glm::normalize(pos)});
	pos = {1.0 * scale_down_ratio,  t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos),glm::normalize(pos)});
	pos = {-1.0 * scale_down_ratio,  -t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos), glm::normalize(pos)});
	pos = {1.0 * scale_down_ratio,  -t * scale_down_ratio,  0.0};
	vertices.push_back(Vertex{ glm::normalize(pos),glm::normalize(pos)});
	
	pos = { 0.0,-1.0 * scale_down_ratio, t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) ,glm::normalize(pos)});
	pos = { 0.0, 1.0 * scale_down_ratio, t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos),glm::normalize(pos)});
	pos = { 0.0,-1.0 * scale_down_ratio,-t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos),glm::normalize(pos)});
	pos = { 0.0, 1.0 * scale_down_ratio,-t * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos),glm::normalize(pos)});
	
	pos = { t * scale_down_ratio, 0.0,-1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos), glm::normalize(pos)});
	pos = { t * scale_down_ratio, 0.0, 1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos),glm::normalize(pos)});
	pos = {-t * scale_down_ratio, 0.0,-1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos),glm::normalize(pos)});
	pos = {-t * scale_down_ratio, 0.0, 1.0 * scale_down_ratio};
	vertices.push_back(Vertex{ glm::normalize(pos) ,glm::normalize(pos)});



//~ // 5 faces around point 0
	add_triangle(indices,0,11, 5);
	add_triangle(indices,0, 5, 1);
	add_triangle(indices,0, 1, 7);
	add_triangle(indices,0, 7,10);
	add_triangle(indices,0,10,11);

//~ 
//~ // 5 adjacent faces
	add_triangle(indices, 1, 5, 9);
	add_triangle(indices, 5,11, 4);
	add_triangle(indices,11,10, 2);
	add_triangle(indices,10, 7, 6);
	add_triangle(indices, 7, 1, 8);

//~ 
//~ // 5 faces around point 3
	add_triangle(indices, 3, 9, 4);
	add_triangle(indices, 3, 4, 2);
	add_triangle(indices, 3, 2, 6);
	add_triangle(indices, 3, 6, 8);
	add_triangle(indices, 3, 8, 9);

//~ 
//~ // 5 adjacent faces
	add_triangle(indices, 4, 9, 5);
	add_triangle(indices, 2, 4,11);
	add_triangle(indices, 6, 2,10);
	add_triangle(indices, 8, 6, 7);
	add_triangle(indices, 9, 8, 1);

	
	Mesh mesh;
	mesh.vertices = vertices;
	mesh.indices = indices;
	//~ vertices.clear();
	return mesh;
}
