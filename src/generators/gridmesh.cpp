#include "gridmesh.h"


GridMesh::GridMesh()
{
	Param<int> param1{"rows", 5};
	paramsInt.push_back(param1);
	
	Param<int> param2{"cols", 5};
	paramsInt.push_back(param2);	
	
	Param<float> param3{"width", 3.0};
	paramsFloat.push_back(param3);

	Param<float> param4{"length", 3.0};
	paramsFloat.push_back(param4);	
}


Mesh GridMesh::generateGrid(int rows, int cols, float width, float length)
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
			
			vert.position.x = (posx - 0.5) * width;
			vert.position.y = (posy - 0.5) * length;
			vert.position.z = 0.0;
			
			vert.t_coords.x = posx;
			vert.t_coords.y = posy;
			
			vert.normal.x = 0.0;
			vert.normal.y = 0.0;
			vert.normal.z = 1.0;			
			
			vertices.push_back(vert);
		}
	}
	
	for (unsigned int y = 0; y < rows-1; y++)
	{
		for (unsigned int x = 0; x < cols-1; x++)
		{
			unsigned int curIndex = x + y * cols;
			indices.push_back(curIndex);
			indices.push_back(curIndex+cols);
			indices.push_back(curIndex+cols+1);
			
			indices.push_back(curIndex+cols+1);
			indices.push_back(curIndex+1);
			indices.push_back(curIndex);			
		}		
	}	
	
	mesh.vertices = vertices;
	mesh.indices = indices;
	
	
	return mesh;
}

GridMesh::~GridMesh()
{
	
}

