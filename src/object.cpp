#include "object.h"
#include "texture.h"



Object::Object() 
	: Entity3D()
{
	//~ std::cout << "--- Object CREATED ---\n";
	
	
	color = glm::vec4(1.0f,1.0f,1.0f,1.0f);	
}

void Object::moveFilter(int _origin, int _dest)
{
	MeshFilter* save = meshFilters[_origin];
	meshFilters.erase(meshFilters.begin()+_origin);
	meshFilters.insert(meshFilters.begin() + _dest, save);
}

void Object::updateMesh()
{
	if(has_generator){
		
		if( mesh_generator->need_update)
		{
			//~ printf("---> Generate Mesh\n");
			mesh = mesh_generator->generate();
			mesh_generator->need_update = false;
			
			// force filters need_update
			for (int i = 0; i < meshFilters.size(); i++)
			{
				meshFilters[i]->need_update = true;
			}
			
			buildVbo();
		}else{
			mesh = mesh_generator->mesh_cache;
		}
	}

	if(hasFilters)
	{		
		//// check first if one of the filters need update
		bool some_need = false;
		for (int i = 0; i < meshFilters.size(); i++)
		{
			if(meshFilters[i]->need_update){
				some_need = true;
			}
		}
				
		for (int i = 0; i < meshFilters.size(); i++)
		{
			if(i == 0 && some_need){
				mesh = mesh_generator->mesh_cache;	
				//~ printf("---> Loading Generator Cache\n");
				some_need = false;
			}					
			
			if(meshFilters[i]->is_active)					
			{
				if(meshFilters[i]->need_update)
				{
					//~ printf("applying filter\n");
					mesh = meshFilters[i]->applyFilter(mesh);
					meshFilters[i]->need_update = false;
				}else{
					mesh = meshFilters[i]->mesh_cache;
					meshFilters[i]->need_update = false;
				}
				
				
			}
			
			meshFilters[i]->need_update = false;
		}
		
		//~ if(some_need == false){
			//~ mesh = mesh_generator->mesh_cache;
		//~ }
		buildVbo();
	}
	
	
}

void Object::init()
{
	//~ printf("--- START object initialization \n");
	//~ initShader();
	buildTexture();	
	buildVbo();	
	//~ printf("--- END object initialization \n");
}

void Object::initShader(){
	
	shader.loadVertexShaderSource("../src/res/shaders/basic_shader.vert");
	shader.loadFragmentShaderSource("../src/res/shaders/basic_shader.frag");	
	
	shader.createShader();	

		//~ lineShader.loadVertexShaderSource("../src/res/shaders/line_shader.vert");
		//~ lineShader.loadFragmentShaderSource("../src/res/shaders/line_shader.frag");		
		//~ lineShader.createShader();			
	//~ printf("--- END shaders initialization \n");
}

void Object::buildTexture()
{	
	texture.load("../src/res/textures/uvgrid.jpg");
	texture.bind();	
}

std::vector<uniform> Object::getShaderUniforms()
{
	
	std::vector<uniform> uniforms;
		
	GLint num_uniforms;
	glGetProgramiv(shader.m_id, GL_ACTIVE_UNIFORMS, &num_uniforms);
	GLchar uniform_name[256];
	GLsizei length;
	GLint size;
	GLenum type;
	
	std::cout << " Num Uniform -->" << num_uniforms << "\n";
	for (int i = 0; i < num_uniforms; i++)
	{
		glGetActiveUniform(shader.m_id, i, sizeof(uniform_name), &length, &size, &type, uniform_name);
		// ... save this uniform data so it can be used during rendering
		//~ std::cout << "\tuniform -->" << uniform_name << "\n";
		
		uniform u;
		std::string str(uniform_name);
		u.uniform_name = str;
		u.length = length;
		u.size = size;
		u.type = type;
		
		uniforms.push_back(u);
	}		
	
	return uniforms;	
}

void Object::buildVbo()
{
	
	//~ printf("--- START VBO initialization \n");
	
	
	
	//~ std::vector<uniform> us;
	//~ us = getShaderUniforms();
	//~ 
	//~ for (int i = 0; i < us.size(); i++)
	//~ {	
		//~ std::cout << "Uniform --> "  << us[i].uniform_name << "\n";
		//~ std::cout << "\tLength --> "  << us[i].length << "\n";
		//~ std::cout << "\tSize --> "  << us[i].size << "\n";
		//~ 
		//~ switch(us[i].type){
			//~ case GL_FLOAT:
				//~ std::cout << "\tType --> float\n";
				//~ break;						
			//~ case GL_FLOAT_VEC2:
				//~ std::cout << "\tType --> vec2\n";
				//~ break;			
			//~ case GL_FLOAT_VEC3:
				//~ std::cout << "\tType --> vec3\n";
				//~ break;
			//~ case GL_FLOAT_VEC4:
				//~ std::cout << "\tType --> vec4\n";
				//~ break;					
			//~ case GL_FLOAT_MAT4:
				//~ std::cout << "\tType --> mat4\n";
				//~ break;					
							//~ 
			//~ case GL_SAMPLER_2D:
				//~ std::cout << "\tType --> sampler2D\n";
				//~ break;								
			//~ default:
				//~ std::cout << "\tType --> UNKNOWN\n";
				//~ break;
		//~ }
		//~ 
	//~ }
	//~ 
	
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
	
	
	//// generate "display normals" data	
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{		
		Vertex curVert = mesh.vertices[i];
		//// enter the vertex actual position
		normals_data.push_back(curVert.position.x);
		normals_data.push_back(curVert.position.y);
		normals_data.push_back(curVert.position.z);
		
		float scale = 0.1;
		//// enter the vertex actual position + normal values
		normals_data.push_back(curVert.position.x + curVert.normal.x * scale);
		normals_data.push_back(curVert.position.y + curVert.normal.y * scale);
		normals_data.push_back(curVert.position.z + curVert.normal.z * scale);		
		
	}
	glDeleteBuffers(1, &m_normals_vbo);
	glGenBuffers(1,&m_normals_vbo);	
	glBindBuffer(GL_ARRAY_BUFFER, m_normals_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals_data.size(), normals_data.data() ,GL_DYNAMIC_DRAW);
	normals_data.clear();

	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	// BBOX
	computeBoundingBox();
	
	glm::vec3 bbpos = boundingBox.position;
	glm::vec3 bbsize = boundingBox.size;
	
	float bbox_data[] = 
	{
		
		// xy square
		bbpos.x, bbpos.y, bbpos.z,
		bbpos.x + bbsize.x, bbpos.y, bbpos.z,
		
		bbpos.x, bbpos.y, bbpos.z,
		bbpos.x, bbpos.y + bbsize.y, bbpos.z,
		
		bbpos.x, bbpos.y + bbsize.y, bbpos.z,
		bbpos.x + bbsize.x, bbpos.y + bbsize.y, bbpos.z,		
		
		bbpos.x + bbsize.x, bbpos.y, bbpos.z,
		bbpos.x + bbsize.x, bbpos.y + bbsize.y, bbpos.z,		
		
		// xy square offset by z
		bbpos.x, bbpos.y, bbpos.z + bbsize.z,
		bbpos.x + bbsize.x, bbpos.y, bbpos.z + bbsize.z,
		
		bbpos.x, bbpos.y, bbpos.z + bbsize.z,
		bbpos.x, bbpos.y + bbsize.y, bbpos.z + bbsize.z,
		
		bbpos.x, bbpos.y + bbsize.y, bbpos.z + bbsize.z,
		bbpos.x + bbsize.x, bbpos.y + bbsize.y, bbpos.z + bbsize.z,		
		
		bbpos.x + bbsize.x, bbpos.y, bbpos.z + bbsize.z,
		bbpos.x + bbsize.x, bbpos.y + bbsize.y, bbpos.z + bbsize.z,
		
		// connecting lines
		bbpos.x, bbpos.y, bbpos.z,
		bbpos.x, bbpos.y, bbpos.z + bbsize.z,
		
		bbpos.x + bbsize.x, bbpos.y, bbpos.z,
		bbpos.x + bbsize.x, bbpos.y, bbpos.z + bbsize.z,
		
		bbpos.x , bbpos.y + bbsize.y, bbpos.z,
		bbpos.x, bbpos.y + bbsize.y, bbpos.z + bbsize.z,	
			
		bbpos.x + bbsize.x, bbpos.y + bbsize.y, bbpos.z,
		bbpos.x + bbsize.x, bbpos.y + bbsize.y, bbpos.z + bbsize.z
	};
	
	glDeleteBuffers(1, &m_bbox_vbo);
	glGenBuffers(1,&m_bbox_vbo);	
	glBindBuffer(GL_ARRAY_BUFFER, m_bbox_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3*12*2, bbox_data ,GL_DYNAMIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	//~ printf("--- END VBO initialization \n");
}



BoundingBox Object::getBoundingBox()
{
	return boundingBox;
}

void Object::computeBoundingBox()
{
	
	//~ std::cout << "Computing BBOX\n";
	float minx = 100000.0;
	float miny = 100000.0;
	float minz = 100000.0;
	float maxx = -100000.0;
	float maxy = -100000.0;
	float maxz = -100000.0;
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		glm::vec3 vpos = mesh.vertices[i].position;
		
		//~ printf("Vertex %d : %.3f, %.3f, %.3f \n", i, vpos.x, vpos.y, vpos.z);
		if(vpos.x < minx)
			minx = vpos.x;
		if(vpos.x > maxx)
			maxx = vpos.x;
		if( vpos.y < miny)
			miny = vpos.y;
		if(vpos.y > maxy)
			maxy = vpos.y;
		if( vpos.z < minz)
			minz = vpos.z;
		if(vpos.z > maxz)
			maxz = vpos.z;			

	}
	
	boundingBox.position = glm::vec3(minx, miny, minz);
	boundingBox.size = glm::vec3(maxx-minx, maxy-miny, maxz-minz);
	
	//~ printf("---------------------------\n");
	//~ printf("minx : %.3f, miny : %.3f, minz : %.3f\n", minx, miny, minz);
	//~ printf("maxx : %.3f, maxy : %.3f, maxz : %.3f\n", maxx, maxy, maxz);
	//~ printf("BBOX position : %.3f, %.3f, %.3f\n", boundingBox.position.x , boundingBox.position.y, boundingBox.position.z);
	//~ printf("BBOX size : %.3f, %.3f, %.3f\n", boundingBox.size.x , boundingBox.size.y, boundingBox.size.z);
	
	
	
}

void Object::printMeshData()
{
	
	
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		std::cout << "Vertex "<< i << ":\n";
		std::cout <<"\tpos :"<< mesh.vertices[i].position.x <<", "<< mesh.vertices[i].position.y <<", "<< mesh.vertices[i].position.z << "\n";
		std::cout <<"\tnormal :"<< mesh.vertices[i].normal.x <<", "<< mesh.vertices[i].normal.y <<", "<< mesh.vertices[i].normal.z << "\n";
		std::cout <<"\t_coords :"<< mesh.vertices[i].t_coords.x <<", "<< mesh.vertices[i].t_coords.y <<"\n";		
	}
	
	
	std::cout << "Indices : \n";
	for (int i = 0; i < mesh.indices.size(); i++)
	{
		std::cout << "\tindex "<< i << ": " <<  mesh.indices[i]<< "\n";
	}
	
	
	
}


void Object::draw(GLuint mode)
{
		

		//~ shader.useProgram();
		
		//~ glEnable(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		texture.bind();
		glUniform1i(glGetUniformLocation(shader.m_id,"u_tex"), 0);
		
		
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); 			
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(3 * sizeof(float))); 			
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(6 * sizeof(float))); 			
		
		
		glEnableVertexAttribArray(0);// position
		glEnableVertexAttribArray(1);// normals
		glEnableVertexAttribArray(2);//t_coords
		
		
		if(mode == GL_POINTS){
			
			glPointSize(3);
			glDrawArrays(mode,0, mesh.vertices.size());		
				
		}else if( mode == GL_TRIANGLES){


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ibo);
			glDrawElements(mode, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
			
			//~ glPointSize(10);
			//~ glDrawArrays(GL_POINTS,0, mesh.vertices.size());					
			
		}
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		
		
		glUseProgram(0);
		glDisable(GL_TEXTURE_2D);
		texture.unbind();
	
		
}


void Object::drawBoundingBox()
{
		//~ lineShader.useProgram();
		//~ glUniform4f(glGetUniformLocation(lineShader.m_id,"u_color"), 1.0,1.0,0.5,1.0);
		glBindBuffer(GL_ARRAY_BUFFER, m_bbox_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0); 					
		glEnableVertexAttribArray(0);		
		
			glDrawArrays(GL_LINES,0, 2*12);
		
		glDisableVertexAttribArray(0);		
		glBindBuffer(GL_ARRAY_BUFFER, 0);	
		
		glUseProgram(0);	
}

void Object::drawNormals()
{
			
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

void Object::drawPoints(){
		//~ lineShader.useProgram();
		//~ glUniform4f(glGetUniformLocation(shader.m_id,"u_color"), 0.0,1.0,0.5,1.0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0); 					
		glEnableVertexAttribArray(0);		
			glPointSize(4);
			glDrawArrays(GL_POINTS,0, mesh.vertices.size());
		
		glDisableVertexAttribArray(0);		
		glBindBuffer(GL_ARRAY_BUFFER, 0);	
		
		glUseProgram(0);	
}

Object::~Object()
{
	delete mesh_generator;
	for (int i = 0; i < meshFilters.size(); i++)
	{
		delete meshFilters[i];
	}
	//~ delete mesh_generator;
	
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
	glDeleteBuffers(1, &m_normals_vbo);
	glDeleteBuffers(1, &m_bbox_vbo);
	
	
	std::cout << "--- Object DELETED  --- \n";
}
