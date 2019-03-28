#include "gizmo.h"

#include "../utils/mesh_utils.h"

Gizmo::Gizmo()
{
	position = glm::vec3(0.0f,0.0f,0.0f);
	rotation = glm::vec3(0.0f,0.0f,0.0f);
	scale = glm::vec3(1.0f,1.0f,1.0f);	
	
	
	Handle trans_1;// = new Handle();
	trans_1.setName("first handle");
	handles.push_back(trans_1);

}

// copy constructor
Gizmo::Gizmo(const Gizmo& other)
{
	
	//~ printf("Gizmo COPY CONSTRUCTOR\n");

	
	transforms = other.transforms;	
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;

	
	setName("copy_name");
}

void Gizmo::buildVbo()
{
	
	//~ printf("--- START VBO initialization \n");
	
	
	MeshUtils utils;
	mesh = utils.cylinderMesh(0.01, 1.0);
	//~ float vertices[18] = {
		//~ 
		//~ 0.0, 0.0, 0.0,
		//~ 1.0, 0.0, 0.0,
		//~ 1.0, 0.1, 0.0,
		//~ 
		//~ 0.0, 0.0, 0.0,
		//~ 1.0, 0.1, 0.0,
		//~ 0.0, 0.1, 0.0
		//~ };
//~ 
	//~ unsigned int indices[6] = {
		//~ 
		//~ 0,1,2,
		//~ 3,4,5
	//~ };
	
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
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), vertex_data.data() ,GL_DYNAMIC_DRAW);
	
	vertex_data.clear();	
	
	glBindBuffer(GL_ARRAY_BUFFER,0);	
	
	glDeleteBuffers(1, &m_ibo);
	glGenBuffers(1, &m_ibo);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), mesh.indices.data() ,GL_DYNAMIC_DRAW);
		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	

	
	//~ printf("--- END VBO initialization \n");
}


void Gizmo::draw(Shader & _shader, Camera & camera){
	//~ shader.useProgram();
		

		glm::mat4 model = glm::mat4(1.0f);
		
		model = target_object->transforms * model;
		
		target_object->applyParentsMatrices(model);
		
		glm::vec3 world_pos = glm::vec3( model[3][0], model[3][1], model[3][2]);
		
		float cam_distance = glm::distance(world_pos, camera.position);
		//~ printf("cam distance -> %.3f \n", cam_distance);
		float dist_square = cam_distance * cam_distance;
		float scale = cam_distance * 0.2;
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		// rotate to lay down X Axis
		model = glm::rotate(model, (float)(PI) * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
		
		
		glUniformMatrix4fv(glGetUniformLocation(_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
		GLuint COLOR_LOC = glGetUniformLocation(_shader.m_id,"u_color");		
		glUniform4f(COLOR_LOC, 1.0, 0.0, 0.0, 1.0);			
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); 			
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void *)(3 * sizeof(float))); 		
			
		
		
		glEnableVertexAttribArray(0);// position
		glEnableVertexAttribArray(1);// normal

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);	
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		

		// rotate to lay down Y Axis
		model = glm::mat4(1.0f);
		model = target_object->transforms * model;
		target_object->applyParentsMatrices(model);
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		model = glm::rotate(model, (float)(PI) * -0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
		COLOR_LOC = glGetUniformLocation(_shader.m_id,"u_color");		
		glUniform4f(COLOR_LOC, 0.0, 1.0, 0.0, 1.0);			
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); 			
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void *)(3 * sizeof(float))); 		
			
		
		
		glEnableVertexAttribArray(0);// position
		glEnableVertexAttribArray(1);// normal

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);	
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		
		// rotate to lay down Z Axis : do not rotate, already in the right place
		model = glm::mat4(1.0f);
		
		model = target_object->transforms * model;
		target_object->applyParentsMatrices(model);
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		//model = glm::rotate(model, (float)(PI) * -0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

		glUniformMatrix4fv(glGetUniformLocation(_shader.m_id,"model"), 1, GL_FALSE, glm::value_ptr(model));	
		COLOR_LOC = glGetUniformLocation(_shader.m_id,"u_color");		
		glUniform4f(COLOR_LOC, 0.0, 0.0, 1.0, 1.0);			
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0); 			
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void *)(3 * sizeof(float))); 		
			
		
		
		glEnableVertexAttribArray(0);// position
		glEnableVertexAttribArray(1);// normal

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);	
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);		
		
		glUseProgram(0);

		
}
void Gizmo::setName(std::string str_name)
{

	for (int i = 0; i < str_name.size(); i++)
	{
		name[i] = str_name[i];
	}
	
}



glm::vec3 Gizmo::getWorldPosition()
{
	glm::vec4 temp = glm::vec4(position.x, position.y, position.z,1.0f);
	glm::mat4 temp_matrix = glm::mat4(1.0f);
	
	temp_matrix = transforms * temp_matrix;
	temp = temp * temp_matrix;
	//~ 
	return glm::vec3(temp.x, temp.y, temp.z);
	
}


Gizmo::~Gizmo()
{
	
}

