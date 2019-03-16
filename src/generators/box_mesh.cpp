#include "box_mesh.h"


BoxMesh::BoxMesh()
{
	setType(BOX_MESH_GENERATOR);

	p_width = new ParamFloat("width",1.0);
	param_layout.push(p_width);

	p_height = new ParamFloat("height",1.0);
	param_layout.push(p_height);	
	
	p_length = new ParamFloat("length",1.0);
	param_layout.push(p_length);		
	
	
	p_segs_width = new ParamInt("segs_width",4);
	param_layout.push(p_segs_width);

	p_segs_height = new ParamInt("segs_height",4);
	param_layout.push(p_segs_height);	
	
	p_segs_length = new ParamInt("segs_length",4);
	param_layout.push(p_segs_length);			
	

}

BoxMesh::BoxMesh(const BoxMesh& other)
{
	setType(BOX_MESH_GENERATOR);

	p_width = new ParamFloat(*other.p_width);
	param_layout.push(p_width);

	p_height = new ParamFloat(*other.p_height);
	param_layout.push(p_height);	
	
	p_length = new ParamFloat(*other.p_length);
	param_layout.push(p_length);		
	
	
	p_segs_width = new ParamInt(*other.p_segs_width);
	param_layout.push(p_segs_width);

	p_segs_height = new ParamInt(*other.p_segs_height);
	param_layout.push(p_segs_height);	
	
	p_segs_length = new ParamInt(*other.p_segs_length);
	param_layout.push(p_segs_length);				
}

static void rotate_vertices(std::vector<Vertex> & verts, float angle, glm::vec3 axis)
{
	glm::mat4 rot_matrix = glm::mat4(1.0f);
	rot_matrix = glm::rotate(rot_matrix, glm::radians(angle), axis);
	for (int i = 0; i < verts.size(); i++)
	{
		glm::vec4 temp(verts[i].position, 1.0f);
		verts[i].position =  rot_matrix * temp;
	}	
}

static void scale_vertices(std::vector<Vertex> & verts, glm::vec3 amount = glm::vec3(1.0f,1.0f,1.0f))
{
	glm::mat4 scale_matrix = glm::mat4(1.0f);
	scale_matrix = glm::scale(scale_matrix, amount);
	for (int i = 0; i < verts.size(); i++)
	{
		glm::vec4 temp(verts[i].position, 1.0f);
		verts[i].position =  scale_matrix * temp;
	}	
}



static void offset_indices(std::vector<unsigned int> & indices, int offset)
{
	for (int i = 0; i < indices.size(); i++)
	{
		indices[i] += offset;
	}
	
}



static TRI_PLANE make_plane(int rows, int cols)
{
	
	TRI_PLANE plane_data;
	// make a unit plane at 0.5 in z to serve as a primitive for every other plane by rotation
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			Vertex vert;
			float posx = 1.0/(cols-1) * x;
			float posy = 1.0/(rows-1) * y;			
			
			vert.position.x = (posx - 0.5);// * width;
			vert.position.y = (posy - 0.5);// * length;
			vert.position.z = 1.0/2.0;
			
			vert.t_coords.x = posx;
			vert.t_coords.y = posy;
			
			vert.normal.x = 0.0;
			vert.normal.y = 0.0;
			vert.normal.z = 1.0;			
			
			plane_data.vertices.push_back(vert);
		}
	}	
	
	for (unsigned int y = 0; y < rows-1; y++)
	{
		for (unsigned int x = 0; x < cols-1; x++)
		{
			unsigned int curIndex = x + y * cols;
			plane_data.indices.push_back(curIndex);
			plane_data.indices.push_back(curIndex+cols+1);
			plane_data.indices.push_back(curIndex+cols);
			
			plane_data.indices.push_back(curIndex+cols+1);
			plane_data.indices.push_back(curIndex);
			plane_data.indices.push_back(curIndex+1);			
		}		
	}		
	
	return plane_data;
}

Mesh BoxMesh::generateBox(float width, float height, float length, int segs_width, int segs_height, int segs_length)
{

	if(segs_width < 2 ) 
		segs_width = 2;
	if(segs_height < 2 ) 
		segs_height = 2;
	if(segs_length < 2 ) 
		segs_length = 2;			
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh mesh;
	
	std::vector<unsigned int> plane_indices;

	int id_offset = 0;

	TRI_PLANE top_plane_data = make_plane(segs_length, segs_width);
	
	std::vector<Vertex> topVerts = top_plane_data.vertices;	
	std::vector<unsigned int> topIDS = top_plane_data.indices;	
	offset_indices(topIDS, id_offset);
	id_offset += segs_length * segs_width;
	
	scale_vertices(topVerts, glm::vec3(width, length, height));
	
	vertices.insert(vertices.end(), topVerts.begin(), topVerts.end());
	indices.insert(indices.end(), topIDS.begin(), topIDS.end());
	
	
	
	TRI_PLANE back_plane_data = make_plane(segs_height, segs_width);
	
	std::vector<Vertex> backVerts = back_plane_data.vertices;	
	std::vector<unsigned int> backIDS = back_plane_data.indices;	
	rotate_vertices(backVerts, -90.0f, glm::vec3(1.0f, 0.0f ,0.0f));	
	scale_vertices(backVerts, glm::vec3(width, length, height));	
	
	vertices.insert(vertices.end(), backVerts.begin(), backVerts.end());
	
	offset_indices(backIDS, id_offset);
	id_offset += segs_height * segs_width;
	
	indices.insert(indices.end(), backIDS.begin(), backIDS.end());
	
	//~ 
	TRI_PLANE left_plane_data = make_plane( segs_length, segs_height);
	
	std::vector<Vertex> leftVerts = left_plane_data.vertices;	
	std::vector<unsigned int> leftIDS = left_plane_data.indices;	
	rotate_vertices(leftVerts, -90.0f, glm::vec3(0.0f, 1.0f ,0.0f));	
	scale_vertices(leftVerts, glm::vec3(width, length, height));	
	
	vertices.insert(vertices.end(), leftVerts.begin(), leftVerts.end());
	
	offset_indices(leftIDS, id_offset);
	id_offset += segs_length * segs_height;
	indices.insert(indices.end(), leftIDS.begin(), leftIDS.end());	
	
	//~ 
	TRI_PLANE right_plane_data = make_plane( segs_length, segs_height);
	
	std::vector<Vertex> rightVerts = right_plane_data.vertices;	
	std::vector<unsigned int> rightIDS = right_plane_data.indices;	
	rotate_vertices(rightVerts, 90.0f, glm::vec3(0.0f, 1.0f ,0.0f));	
	scale_vertices(rightVerts, glm::vec3(width, length, height));	
	
	vertices.insert(vertices.end(), rightVerts.begin(), rightVerts.end());
	
	offset_indices(rightIDS, id_offset);
	id_offset += segs_length * segs_height;
	indices.insert(indices.end(), rightIDS.begin(), rightIDS.end());		
	
	//~ 
	TRI_PLANE bottom_plane_data = make_plane( segs_length, segs_width);
	
	std::vector<Vertex> bottomVerts = bottom_plane_data.vertices;	
	std::vector<unsigned int> bottomIDS = bottom_plane_data.indices;	
	rotate_vertices(bottomVerts, 180.0f, glm::vec3(1.0f, 0.0f ,0.0f));	
	scale_vertices(bottomVerts, glm::vec3(width, length, height));	
	
	vertices.insert(vertices.end(), bottomVerts.begin(), bottomVerts.end());
	
	offset_indices(bottomIDS, id_offset);
	id_offset += segs_length * segs_width;
	indices.insert(indices.end(), bottomIDS.begin(), bottomIDS.end());	
	
	
	TRI_PLANE front_plane_data = make_plane(segs_height, segs_width);
	
	std::vector<Vertex> frontVerts = front_plane_data.vertices;	
	std::vector<unsigned int> frontIDS = front_plane_data.indices;	
	rotate_vertices(frontVerts, 90.0f, glm::vec3(1.0f, 0.0f ,0.0f));	
	scale_vertices(frontVerts, glm::vec3(width, length, height));	
	
	vertices.insert(vertices.end(), frontVerts.begin(), frontVerts.end());
	
	offset_indices(frontIDS, id_offset);
	id_offset += segs_height * segs_width;
	
	indices.insert(indices.end(), frontIDS.begin(), frontIDS.end());	

	
	
	
	mesh.vertices = vertices;
	mesh.indices = indices;
	
	mesh.computeNormals();
	
	return mesh;
}

BoxMesh::~BoxMesh()
{
	
}

