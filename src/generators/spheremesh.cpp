#include "spheremesh.h"

#define PI 3.14159265359
SphereMesh::SphereMesh()
{

	
	p_cols = new ParamInt("cols", 24);
	param_layout.push(p_cols);
	p_rows = new ParamInt("rows", 16);
	param_layout.push(p_rows);
	
	p_radius = new ParamFloat("radius", 0.5);
	param_layout.push(p_radius);
	
	p_u_ratio = new ParamFloat("U ratio", 1.0);
	param_layout.push(p_u_ratio);
	
	p_v_ratio = new ParamFloat("V ratio", 1.0);
	param_layout.push(p_v_ratio);		
}


SphereMesh::~SphereMesh()
{
	
}

static glm::vec3 fromPolar(float u, float v, float radius){
	glm::vec3 temp;
	
	temp.x = radius * sin(u) * cos(v);
	temp.y = radius * sin(u) * sin(v);
	temp.z = radius * cos(u);
	
	return temp;
}

static Mesh generateGrid(int rows, int cols, float radius)
{
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh mesh;
	
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			Vertex vert;
			float posx = 1.0/(cols-1) * x;
			float posy = 1.0/(rows-1) * y;			
			
			
			vert.position = fromPolar(posx * PI, posy * 2.0 * PI, radius);
			//~ vert.position.x = (posx );
			//~ vert.position.y = (posy );
			//~ vert.position.z = 0.0;
			
			vert.t_coords.x = posx;
			vert.t_coords.y = posy;
			
			vert.normal = glm::normalize( vert.position);			
			
			vertices.push_back(vert);
		}
	}
	
	for (unsigned int y = 0; y < rows-1; y++)
	{
		for (unsigned int x = 0; x < cols-1; x++)
		{
			unsigned int curIndex = x + y * cols;
			indices.push_back(curIndex);
			indices.push_back(curIndex+cols+1);
			indices.push_back(curIndex+cols);
			
			indices.push_back(curIndex+cols+1);
			indices.push_back(curIndex);
			indices.push_back(curIndex+1);			
		}		
	}	
	
	mesh.vertices = vertices;
	mesh.indices = indices;
	
	
	return mesh;
}


Mesh SphereMesh::generateSphere(int rows, int cols, float radius, float u_ratio, float v_ratio)
{
	

	
	
	Mesh mesh;
	
	mesh = generateGrid(rows, cols, radius);
	return mesh;
} 
