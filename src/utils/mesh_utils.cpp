#include "mesh_utils.h"


MeshUtils::MeshUtils()
{
	
	
	
}

Mesh MeshUtils::cylinderMesh(float radius, float height, int segs_height, int segs_radius)
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
	
	mesh.computeNormals();
	return mesh;
}


Mesh MeshUtils::coneMesh(float radius1, float radius2, float height, int segs_height, int segs_radius)
{	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh mesh;
	
	// make cylinder grid 
	for (int y = 0; y < segs_height; y++)
	{
		float height_ratio = (1.0/(segs_height-1)) * y;
		// lerp radius by height_ratio
		float radius = (radius2 - radius1) * height_ratio + radius1;
		for (int x = 0; x < segs_radius; x++)
		{
			Vertex vert;
			float ratio = 1.0/(segs_radius-1) * x;
					
			
			vert.position.x = cos(ratio * PI * 2.0) * radius;
			vert.position.y = sin(ratio * PI * 2.0) * radius;
			vert.position.z = ((1.0/(segs_height-1)) * y) * height;
			
			vert.t_coords.x = ratio;
			vert.t_coords.y = height_ratio;
			
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
		vert.position.x = cos(ratio * PI * 2.0) * radius2;
		vert.position.y = sin(ratio * PI * 2.0) * radius2;
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
		vert.position.x = cos(ratio * PI * 2.0) * radius1;
		vert.position.y = sin(ratio * PI * 2.0) * radius1;
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
	
	mesh.computeNormals();
	return mesh;
}

Mesh MeshUtils::merge(Mesh& mesh1, Mesh& mesh2)
{
	Mesh A = mesh1;
	Mesh B = mesh2;
	Mesh AB;
	
	AB.vertices.insert( AB.vertices.end(), A.vertices.begin(), A.vertices.end());
	AB.indices.insert( AB.indices.end(), A.indices.begin(), A.indices.end());

	// offset indices in mesh2
	for (int i = 0; i < B.indices.size(); i++)
	{
		B.indices[i] += A.vertices.size();
	}
	
	AB.vertices.insert( AB.vertices.end(), B.vertices.begin(), B.vertices.end());
	AB.indices.insert( AB.indices.end(), B.indices.begin(), B.indices.end());
	
	//~ printf("\nVertex Data -------------------\n");
	for (int i = 0; i < AB.vertices.size(); i++)
	{
		Vertex vert = AB.vertices[i];
		//~ printf("%d\t : %.3f, %.3f, %.3f\n", i, vert.position.x, vert.position.y, vert.position.z);
	}
	//~ printf("\nIndices Data -------------------\n");
	for (int i = 0; i < AB.indices.size(); i++)
	{
		int index = AB.indices[i];
		//~ printf("%d\t : %d\n", i, index);
	}	
	
	return AB;
}
