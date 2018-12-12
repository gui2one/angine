#include "gridmesh.h"


GridMesh::GridMesh()
{
	
}

Mesh GridMesh::generate(int rows, int cols, float width, float length){
	
	
	
	Mesh mesh;
	std::vector<Vertex> vertices;
	
	
	for (int x = 0; x < cols-1; x++)
	{
		for (int y = 0; y < rows-1; y++)
		{
			
			//// first triangle
			float xpos = (float)((1.0/(cols-1)) * x - 0.5 ) * width;
			float ypos = (float)((1.0/(rows-1)) * y - 0.5) * length;
			vertices.push_back({
						glm::vec3(xpos,ypos,0.0),
						glm::vec3(0.0,1.0,0.0), 
						glm::vec2((float)(1.0/(cols-1)) * x,(float)(1.0/(rows-1)) * y)
					});
			xpos = (float)((1.0/(cols-1)) * x + (1.0/(cols-1)) - 0.5 )  * width;
			ypos = (float)((1.0/(rows-1)) * y - 0.5 )  * length;
			vertices.push_back({
						glm::vec3(xpos,ypos,0.0),
						glm::vec3(0.0,1.0,0.0),
						glm::vec2((float)(1.0/(cols-1)) * x + (1.0/(cols-1)),(float)(1.0/(rows-1)) * y)
					});
			xpos = (float)((1.0/(cols-1)) * x  - 0.5 )  * width;
			ypos = (float)((1.0/(rows-1)) * y + (1.0/(rows-1)) - 0.5 ) * length;	
			vertices.push_back({
						glm::vec3(xpos, ypos, 0.0),
						glm::vec3(0.0,1.0,0.0),
						glm::vec2((float)(1.0/(cols-1)) * x,(float)(1.0/(rows-1)) * y + (1.0/(rows-1)))
					});
					
			//~ //second triangle
			xpos = ((float)(1.0/(cols-1)) * x + (1.0/(cols-1))-0.5) * width ;
			ypos = ((float)(1.0/(rows-1)) * y -0.5) * length;			
			vertices.push_back({
						glm::vec3(xpos, ypos,0.0),
						glm::vec3(0.0,1.0,0.0), 
						glm::vec2((float)(1.0/(cols-1)) * x + (1.0/(cols-1)),(float)(1.0/(rows-1)) * y)
					});							
			xpos = ((float)(1.0/(cols-1)) * x + (1.0/(cols-1))-0.5) * width;
			ypos = ((float)(1.0/(rows-1)) * y + (1.0/(rows-1))-0.5) * length;		
			vertices.push_back({
						glm::vec3(xpos, ypos,0.0),
						glm::vec3(0.0,1.0,0.0), 
						glm::vec2((float)(1.0/(cols-1)) * x + (1.0/(cols-1)),(float)(1.0/(rows-1)) * y + (1.0/(rows-1)))
					});	
			xpos = ((float)(1.0/(cols-1)) * x-0.5) * width;
			ypos = ((float)(1.0/(rows-1)) * y + (1.0/(rows-1))-0.5) * length;
			vertices.push_back({
						glm::vec3(xpos, ypos,0.0),
						glm::vec3(0.0,1.0,0.0), 
						glm::vec2((float)(1.0/(cols-1)) * x,(float)(1.0/(rows-1)) * y + (1.0/(rows-1)))
					});						
		}
		
	}
	
	//~ vertices.push_back({glm::vec3(-0.5,-0.5,0.0), glm::vec3(0.0,1.0,0.0), glm::vec2(0.0,0.0)});
	//~ vertices.push_back({glm::vec3(0.5,-0.5,0.0), glm::vec3(0.0,1.0,0.0), glm::vec2(1.0,0.0)});
	//~ vertices.push_back({glm::vec3(0.0,0.5,0.0), glm::vec3(0.0,1.0,0.0), glm::vec2(0.5,1.0)});
	
	
	mesh.vertices = vertices;
	return mesh;
}

GridMesh::~GridMesh()
{
	
}

