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

json ObjectDummy::toJSON()
{
	json j;
	j["type"] = "OBJECT_DUMMY";
	j["name"] = name;	

	j["transforms"] = {
		transforms[0][0], transforms[0][1],transforms[0][2],transforms[0][3],
		transforms[1][0], transforms[1][1],transforms[1][2],transforms[1][3],
		transforms[2][0], transforms[2][1],transforms[2][2],transforms[2][3],
		transforms[3][0], transforms[3][1],transforms[3][2],transforms[3][3]
		
	};

	j["transforms_params"] = param_layout.toJSON();
	j["position"] = {position.x, position.y, position.z};
	j["rotation"] = {rotation.x, rotation.y, rotation.z};
	j["scale"] = {scale.x, scale.y, scale.z};	
	return j;
}


