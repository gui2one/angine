#include "cylinder_mesh.h"


CylinderMesh::CylinderMesh()
{
	Param<float> param1{"radius", 0.5};
	paramsFloat.push_back(param1);
	
	Param<float> param2{"height", 2};
	paramsFloat.push_back(param2);	
	
	Param<int> param3{"segs_height", 3};
	paramsInt.push_back(param3);

	Param<int> param4{"segs_radius", 20};
	paramsInt.push_back(param4);	
}


Mesh CylinderMesh::generateCylinder(float radius, float height, int segs_height, int segs_radius)
{	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh mesh;
	
	// make cylinder grid 
	for (int y = 0; y < segs_height; y++)
	{
		for (int x = 0; x < segs_radius; x++)
		{
			Vertex vert;
			float ratio = 1.0/(segs_radius-1) * x;
					
			
			vert.position.x = cos(ratio * PI * 2.0) * radius;
			vert.position.y = sin(ratio * PI * 2.0) * radius;
			vert.position.z = ((1.0/(segs_height-1)) * y) * height;
			
			vert.t_coords.x = ratio;
			vert.t_coords.y = (1.0/(segs_height-1)) * y;
			
			vert.normal.x = 0.0;
			vert.normal.y = 0.0;
			vert.normal.z = 1.0;			
			
			vertices.push_back(vert);
		}
	}
	
	for (unsigned int y = 0; y < segs_height-1; y++)
	{
		for (unsigned int x = 0; x < segs_radius-1; x++)
		{
			unsigned int curIndex = x + y * segs_radius;
			indices.push_back(curIndex);
			indices.push_back(curIndex+segs_radius+1);
			indices.push_back(curIndex+segs_radius);
			
			indices.push_back(curIndex+segs_radius+1);
			indices.push_back(curIndex);
			indices.push_back(curIndex+1);			
		}		
	}
	
	int cur_num_verts = vertices.size();
	// build top cap
	
	for (int i = 0; i < segs_radius; i++)
	{
		float ratio = 1.0/(segs_radius-1) * i;
		
		Vertex vert;
		vert.position.x = cos(ratio * PI * 2.0) * radius;
		vert.position.y = sin(ratio * PI * 2.0) * radius;
		vert.position.z = height;

		vert.t_coords.x = 0.5;
		vert.t_coords.y = 0.5;		
		
		vert.normal = glm::vec3(0.0f,1.0f,0.0f);
		
		vertices.push_back(vert);		
	

		
	}
	
	// last top cap vertex	
	
	Vertex vert;
	vert.position.x = 0.0;
	vert.position.y = 0.0;
	vert.position.z = height;	
	
	vert.t_coords.x = 0.5;
	vert.t_coords.y = 0.5;		
	
	vert.normal = glm::vec3(0.0f,1.0f,0.0f);				
		
	vertices.push_back(vert);
		
	for (int i = 0; i < segs_radius-1; i++)
	{
		
		indices.push_back(i + (segs_radius * segs_height));
		indices.push_back(i + (segs_radius * segs_height)+1);
		indices.push_back((segs_radius * segs_height) + segs_radius);
	}
	
	// build bottom cap
	
	for (int i = 0; i < segs_radius; i++)
	{
		float ratio = 1.0/(segs_radius-1) * i;
		
		Vertex vert;
		vert.position.x = cos(ratio * PI * 2.0) * radius;
		vert.position.y = sin(ratio * PI * 2.0) * radius;
		vert.position.z = 0.0;

		vert.t_coords.x = 0.5;
		vert.t_coords.y = 0.5;		
		
		vert.normal = glm::vec3( 0.0f, 0.0f, 1.0f);
		
		vertices.push_back(vert);		
	

		
	}
	
	// last bottom cap vertex	
	
	
	vert.position.x = 0.0;
	vert.position.y = 0.0;
	vert.position.z = 0.0;	
	
	vert.t_coords.x = 0.5;
	vert.t_coords.y = 0.5;		
	
	vert.normal = glm::vec3( 0.0f, 0.0f,-1.0f);				
		
	vertices.push_back(vert);
		
	for (int i = 0; i < segs_radius-1; i++)
	{
		
		indices.push_back(i + (segs_radius * (segs_height+1))+2);
		indices.push_back(i + (segs_radius * (segs_height+1))+1);
		indices.push_back(((segs_radius * segs_height) + segs_radius*2)+1);
	}	
	
	
	mesh.vertices = vertices;
	mesh.indices = indices;
	
	
	return mesh;
}

CylinderMesh::~CylinderMesh()
{
	
}

