#include "handle.h"

#include "../utils/mesh_utils.h"
Handle::Handle()
{
	//~ printf("Creating handle\n");
	setName("default handle name");
}

TranslateHandle::TranslateHandle():Handle()
{
	printf("Creating new TranslateHandle\n");
	setName("translate_x");
	
}

void TranslateHandle::buildVbo()
{
	//~ printf("Creating handle's Vertex Buffer Object\n");
	
	MeshUtils utils;
	mesh = utils.cylinderMesh(0.01, 1.0);

	

	vertex_data.clear();	
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		vertex_data.push_back( mesh.vertices[i].position.x);
		vertex_data.push_back( mesh.vertices[i].position.y);
		vertex_data.push_back( mesh.vertices[i].position.z);		
		
		vertex_data.push_back( mesh.vertices[i].normal.x );
		vertex_data.push_back( mesh.vertices[i].normal.y );
		vertex_data.push_back( mesh.vertices[i].normal.z );

	}
	
	
	glDeleteBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo);
	
	printf("\tm_vbo %d\n", m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), vertex_data.data() ,GL_DYNAMIC_DRAW);
	
	
	
		
	
	
	glDeleteBuffers(1, &m_ibo);
	glGenBuffers(1, &m_ibo);	
	
	printf("\tm_ibo %d\n", m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), mesh.indices.data() ,GL_DYNAMIC_DRAW);
		
		
	glBindBuffer(GL_ARRAY_BUFFER,0);		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}


void TranslateHandle::draw(Shader & _shader, Camera & camera, glm::mat4 model_matrix)
{
	//~ 
	
		//~ printf("drawing translate handle !!!!!\n");
		if(mesh.vertices.size() == 0){
			buildVbo();
		}
		
		if( axis == 0 /* x axis */)
		{
			model_matrix = glm::rotate(model_matrix, (float)(PI) * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
			GLuint COLOR_LOC = glGetUniformLocation(_shader.m_id,"u_color");		
			GLCall(glUniform4f(COLOR_LOC, 1.0, 0.0, 0.0, 1.0));			
			
		}else if(axis == 1){
			GLuint COLOR_LOC = glGetUniformLocation(_shader.m_id,"u_color");		
			GLCall(glUniform4f(COLOR_LOC, 0.0, 1.0, 0.0, 1.0));						
			model_matrix = glm::rotate(model_matrix, (float)(PI) * 0.5f, glm::vec3(-1.0f, 0.0f, 0.0f));
			
		}else if(axis == 2){
			
			GLuint COLOR_LOC = glGetUniformLocation(_shader.m_id,"u_color");		
			GLCall(glUniform4f(COLOR_LOC, 0.0, 0.0, 1.0, 1.0));						
			// no rotation, it's build pointing to z axis already
			
			
		}
		////
		glUniformMatrix4fv(glGetUniformLocation(_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model_matrix));	
		//~ GLCall(_shader.useProgram());
		
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
		
		GLCall(
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0)
		);
		GLCall(
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void *)(3 * sizeof(float)))
		);
			
		
		
		GLCall(
			glEnableVertexAttribArray(0)
		);// position
		
		GLCall(
			glEnableVertexAttribArray(1)
		);// normal
		

	

			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo));
			GLCall(glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr));	
		
		GLCall(glDisableVertexAttribArray(0));
		GLCall(glDisableVertexAttribArray(1));
		

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0));

		

		
}
