#include "object_dummy.h"


ObjectDummy::ObjectDummy()
	: Entity3D()
{
	
}

ObjectDummy::ObjectDummy(const ObjectDummy& other) : Entity3D(other){
	//~ printf("ObjectDummy COPY CONSTRUCTOR\n");
	
	//~ init();
}


void ObjectDummy::init()
{
	buildVbo();
}

void ObjectDummy::buildVbo()
{
	std::vector<float> vertex_data;
		
	vertex_data.push_back(-0.5);
	vertex_data.push_back( 0.0);
	vertex_data.push_back( 0.0);
	
	vertex_data.push_back( 0.5);
	vertex_data.push_back( 0.0);
	vertex_data.push_back( 0.0);
	
	
	vertex_data.push_back( 0.0);
	vertex_data.push_back(-0.5);
	vertex_data.push_back( 0.0);

	vertex_data.push_back( 0.0);
	vertex_data.push_back( 0.5);
	vertex_data.push_back( 0.0);
	

	vertex_data.push_back( 0.0);
	vertex_data.push_back( 0.0);
	vertex_data.push_back(-0.5);

	vertex_data.push_back( 0.0);
	vertex_data.push_back( 0.0);
	vertex_data.push_back( 0.5);
	
	//~ printf("data size :: %d\n", vertex_data.size());
	
	glDeleteBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_data.size(), vertex_data.data() ,GL_STATIC_DRAW);	
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void ObjectDummy::draw()
{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); 					
		glEnableVertexAttribArray(0);		
		
			glDrawArrays(GL_LINES,0, 2*3*3);
		
		glDisableVertexAttribArray(0);		
		glBindBuffer(GL_ARRAY_BUFFER, 0);	
		
		glUseProgram(0);		
}




