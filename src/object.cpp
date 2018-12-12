#include "object.h"
#include "texture.h"
//~ #include <memory>
Object::Object(){
	std::cout << "--- Object CREATED ---\n";
	
	position = new glm::vec3(0.0f,0.0f,0.0f);
	rotation = new glm::vec3(0.0f,0.0f,0.0f);
	scale = new glm::vec3(1.0f,1.0f,1.0f);	
	color = new glm::vec4(1.0f,1.0f,1.0f,1.0f);
	texture = new Texture();
	
	
}

void Object::buildTexture(){
	
	
	texture->load("../src/res/textures/uvgrid.jpg");
	texture->bind();	
}

void Object::buildVbo()
{
	
	
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		//~ int id = mesh.indices[i];
		vertex_data.push_back( mesh.vertices[i].position.x);
		vertex_data.push_back( mesh.vertices[i].position.y);
		vertex_data.push_back( mesh.vertices[i].position.z);
		
		vertex_data.push_back( mesh.vertices[i].normal.x);
		vertex_data.push_back( mesh.vertices[i].normal.y);
		vertex_data.push_back( mesh.vertices[i].normal.z);
		
		vertex_data.push_back( mesh.vertices[i].t_coords.x);
		vertex_data.push_back( mesh.vertices[i].t_coords.y);
		
		//~ std::cout << "t__coord__x --> " << mesh.vertices[i].t_coords.x << "\n";
		//~ std::cout << "t__coord__y --> " << mesh.vertices[i].t_coords.y << "\n";
		//~ std::cout <<" ----- \n";
		
	}
	
	
	
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), vertex_data.data() ,GL_STATIC_DRAW);
	
	vertex_data.clear();
	//~ std::cout <<  (mesh.vertices.size()) << "\n";
	
	

	//~ // position attribute, location = 0
	//~ glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); 	
	//~ //normal attribute, location = 1
	//~ glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(3 * sizeof(float))); 	
	//~ //t_coords attribute, location = 2
	//~ glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(6 * sizeof(float))); 		
	
	//~ glEnableVertexAttribArray(0);
	//~ glEnableVertexAttribArray(1);
	//~ glEnableVertexAttribArray(2);
	
	
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	
	
	
	//// generate "display normals" data
	
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		
		
		Vertex curVert = mesh.vertices[i];
		//// enter the vertex actual position
		normals_data.push_back(curVert.position.x);
		normals_data.push_back(curVert.position.y);
		normals_data.push_back(curVert.position.z);
		
		float scale = 0.1;
		normals_data.push_back(curVert.position.x + curVert.normal.x * scale);
		normals_data.push_back(curVert.position.y + curVert.normal.y * scale);
		normals_data.push_back(curVert.position.z + curVert.normal.z * scale);		
		
	}
	glGenBuffers(1,&m_normals_vbo);	
	glBindBuffer(GL_ARRAY_BUFFER, m_normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals_data.size(), normals_data.data() ,GL_STATIC_DRAW);
	normals_data.clear();
	//~ normals_data.clear();
	
	//~ glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
	//~ glEnableVertexAttribArray(0);
	
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	

	
}
void Object::printMeshData(){
	
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		std::cout << "Vertex "<< i << ":\n";
		std::cout <<"\tpos :"<< mesh.vertices[i].position.x <<", "<< mesh.vertices[i].position.y <<", "<< mesh.vertices[i].position.z << "\n";
		std::cout <<"\tnormal :"<< mesh.vertices[i].normal.x <<", "<< mesh.vertices[i].normal.y <<", "<< mesh.vertices[i].normal.z << "\n";
		std::cout <<"\t_coords :"<< mesh.vertices[i].t_coords.x <<", "<< mesh.vertices[i].t_coords.y <<"\n";
		
	}
	
	
}


void Object::draw(){
		

		shader.useProgram();
		
		//~ glEnable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		texture->bind();
		glUniform1i(glGetUniformLocation(shader.m_id,"u_tex"), 0);
		
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); 			
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(3 * sizeof(float))); 			
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(6 * sizeof(float))); 			
		glEnableVertexAttribArray(0);// position
		glEnableVertexAttribArray(1);// normals
		glEnableVertexAttribArray(2);//t_coords
		
			glPointSize(3);
			glDrawArrays(GL_TRIANGLES,0, mesh.vertices.size()*8);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		
		
		glUseProgram(0);
		glDisable(GL_TEXTURE_2D);
		texture->unbind();
	
		
}

void Object::drawNormals(){
			
		shader.useProgram();
		glUniform4f(glGetUniformLocation(shader.m_id,"u_color"), 0.0,1.0,0.5,1.0);
		glBindBuffer(GL_ARRAY_BUFFER, m_normals_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); 					
		glEnableVertexAttribArray(0);		
		
			glDrawArrays(GL_LINES,0, mesh.vertices.size()*2);
		
		glDisableVertexAttribArray(0);		
		glBindBuffer(GL_ARRAY_BUFFER, 0);	
		
		glUseProgram(0);
}

Object::~Object(){
	
	delete position;
	delete rotation;
	delete scale;
	delete texture;
	std::cout << "--- Object DELETED  --- \n";
}
