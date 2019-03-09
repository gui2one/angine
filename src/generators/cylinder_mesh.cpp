#include "cylinder_mesh.h"


CylinderMesh::CylinderMesh(): MeshGenerator()
{	
	
	param_radius = new ParamFloat("radius", 0.666); 	
	param_layout.push(param_radius);
	
	param_height = new ParamFloat("height", 0.2); 	
	param_layout.push(param_height);	
	
	param_segs_height = new ParamInt("segs_height", 5, 2); 	
	param_layout.push(param_segs_height);	
	
	param_segs_radius = new ParamInt("segs_radius", 30, 4); 	
	param_layout.push(param_segs_radius);	
}


CylinderMesh::CylinderMesh(const CylinderMesh& other):MeshGenerator(other){
	//~ printf("CylinderMesh COPY CONSTRUCTOR\n");
	param_radius = new ParamFloat(*other.param_radius); 
	//~ param_radius->setValue(other.param_radius->value);
	param_layout.push(param_radius);
	
	param_height = new ParamFloat(*other.param_height); 	
	//~ param_height->setValue(other.param_height->value);
	param_layout.push(param_height);	
	
	param_segs_height = new ParamInt(*other.param_segs_height); 	
	//~ param_segs_height->setValue(other.param_segs_height->value);
	param_layout.push(param_segs_height);	
	
	param_segs_radius = new ParamInt(*other.param_segs_radius); 	
	//~ param_segs_radius->setValue(other.param_segs_radius->value);
	param_layout.push(param_segs_radius);
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
	
	mesh.computeNormals();
	return mesh;
}

CylinderMesh::~CylinderMesh()
{
	//~ delete param_radius;	
	//~ delete param_height;
	//~ delete param_segs_height;
	//~ delete param_segs_radius;	
	
}

