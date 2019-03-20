#include "object.h"
#include "texture.h"



Object::Object(): Entity3D()
{
	//~ std::cout << "--- Object CREATED ---\n";
	
	
	color = glm::vec4(1.0f,1.0f,1.0f,1.0f);	
}

Object::Object(const Object& other): Entity3D(other){
	//~ printf("-------> copying Object members <------------\n");
	init();
	mesh = other.mesh;
	shader = other.shader;
	color = other.color;	
	
	
	
	//~ mesh_generator = new MeshGenerator((const MeshGenerator&)*other.mesh_generator);
	//~ has_generator = true;
	//~ mesh.vertices.clear();
	//~ mesh.indices.clear();
	//~ mesh_generator->generate();	
	MeshGenerator * gen_ptr = other.mesh_generator;
	SphereMesh * sphere_ptr = nullptr;
	GeoSphereMesh * geosphere_ptr = nullptr;
	GridMesh * grid_ptr = nullptr;
	BoxMesh * box_ptr = nullptr;
	CylinderMesh * cyl_ptr = nullptr;
	FileMesh * file_ptr = nullptr;
	
	if( sphere_ptr = dynamic_cast<SphereMesh*>(gen_ptr)){
		//~ printf("this is a SphereMesh generator type\n");
		mesh_generator = new SphereMesh((const SphereMesh&)*other.mesh_generator);
		generator_type = 1;
		has_generator = true;
		
		mesh.vertices.clear();
		mesh.indices.clear();
		mesh_generator->generate();
		
	}else if( geosphere_ptr = dynamic_cast<GeoSphereMesh*>(gen_ptr)){
		//~ printf("this is a GeoSphereMesh generator type\n");
		mesh_generator = new GeoSphereMesh((const GeoSphereMesh&)*other.mesh_generator);
		generator_type = 2;
		has_generator = true;
		
		mesh.vertices.clear();
		mesh.indices.clear();
		mesh_generator->generate();
		
	}else if( grid_ptr = dynamic_cast<GridMesh*>(gen_ptr)){
		//~ printf("this is a GridMesh Mesh generator type\n");
		mesh_generator = new GridMesh((const GridMesh&)*other.mesh_generator);
		generator_type = 3;
		has_generator = true;
		
		mesh.vertices.clear();
		mesh.indices.clear();
		mesh_generator->generate();
		
	}else if( box_ptr = dynamic_cast<BoxMesh*>(gen_ptr)){
		//~ printf("this is a BoxMesh generator type\n");
		mesh_generator = new BoxMesh((const BoxMesh&)*other.mesh_generator);
		generator_type = 4;
		has_generator = true;
		
		mesh.vertices.clear();
		mesh.indices.clear();
		mesh_generator->generate();
		
	}else if( cyl_ptr = dynamic_cast<CylinderMesh*>(gen_ptr)){
		//~ printf("this is a cylinderMesh generator type\n");
		mesh_generator = new CylinderMesh((const CylinderMesh&)*other.mesh_generator);
		generator_type = 5;
		has_generator = true;
		
		mesh.vertices.clear();
		mesh.indices.clear();
		mesh_generator->generate();
		
	}else if( file_ptr = dynamic_cast<FileMesh*>(gen_ptr)){
		//~ printf("this is a FileMesh generator type\n");
		mesh_generator = new FileMesh((const FileMesh&)*other.mesh_generator);
		generator_type = 6;
		has_generator = true;
		
		mesh.vertices.clear();
		mesh.indices.clear();
		mesh_generator->generate();
		
	}

	for (int i = 0; i < other.meshFilters.size(); i++)
	{
		//~ MeshFilter mesh_filter = *other.meshFilters[i];
		//~ meshFilters.push_back(&mesh_filter);
		
		MeshFilter * filter_ptr = other.meshFilters[i];
		TransformMeshFilter * transform_ptr = nullptr;
		InflateMeshFilter * inflate_ptr = nullptr;
		TwistMeshFilter * twist_ptr = nullptr;
		ComputeNormalsMeshFilter * compute_normals_ptr = nullptr;
		SpherifyMeshFilter * spherify_ptr = nullptr;
		DuplicateMeshFilter * duplicate_ptr = nullptr;
		FromPolarMeshFilter * from_polar_ptr = nullptr;
		MirrorMeshFilter * mirror_ptr = nullptr;
		
		if(transform_ptr = dynamic_cast<TransformMeshFilter*>(filter_ptr)){
			//~ printf("this is a TransformMeshFilter ; yes !!!!!\n");
			TransformMeshFilter * new_filter = new TransformMeshFilter((const TransformMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}else if(inflate_ptr = dynamic_cast<InflateMeshFilter*>(filter_ptr)){
			//~ printf("this is a InflateMeshFilter ; yes !!!!!\n");
			InflateMeshFilter * new_filter = new InflateMeshFilter((const InflateMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}else if(twist_ptr = dynamic_cast<TwistMeshFilter*>(filter_ptr)){
			//~ printf("this is a TwistMeshFilter ; yes !!!!!\n");
			TwistMeshFilter * new_filter = new TwistMeshFilter((const TwistMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}else if(compute_normals_ptr = dynamic_cast<ComputeNormalsMeshFilter*>(filter_ptr)){
			//~ printf("this is a Compute Normals Filter ; yes !!!!!\n");
			ComputeNormalsMeshFilter * new_filter = new ComputeNormalsMeshFilter((const ComputeNormalsMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}else if(spherify_ptr = dynamic_cast<SpherifyMeshFilter*>(filter_ptr)){
			//~ printf("this is a Compute Normals Filter ; yes !!!!!\n");
			SpherifyMeshFilter * new_filter = new SpherifyMeshFilter((const SpherifyMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}else if(duplicate_ptr = dynamic_cast<DuplicateMeshFilter*>(filter_ptr)){
			//~ printf("this is a DuplicateMeshFilter ; yes !!!!!\n");
			DuplicateMeshFilter * new_filter = new DuplicateMeshFilter((const DuplicateMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}else if(from_polar_ptr = dynamic_cast<FromPolarMeshFilter*>(filter_ptr)){
			//~ printf("this is a FromPolarMeshFilter ; yes !!!!!\n");
			FromPolarMeshFilter * new_filter = new FromPolarMeshFilter((const FromPolarMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}else if(mirror_ptr = dynamic_cast<MirrorMeshFilter*>(filter_ptr)){
			//~ printf("this is a MirrorMeshFilter ; yes !!!!!\n");
			MirrorMeshFilter * new_filter = new MirrorMeshFilter((const MirrorMeshFilter&)*other.meshFilters[i]);
			new_filter->applyFilter(mesh);
			meshFilters.push_back(new_filter);
			hasFilters = true;
		}
	}
	
	
	
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

void Object::initShader()
{
	
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

BoundingBox Object::computeAABB()
{
	//~ std::cout << "Computing BBOX\n";
	
	glm::mat4 all_transforms = glm::mat4(1.0f);
	
	applyParentsMatrices(all_transforms);
	all_transforms *= transforms;
	float minx = 100000.0;
	float miny = 100000.0;
	float minz = 100000.0;
	float maxx = -100000.0;
	float maxy = -100000.0;
	float maxz = -100000.0;
	for (int i = 0; i < mesh.vertices.size(); i++)
	{
		glm::vec3 vert_pos = mesh.vertices[i].position;
		glm::vec4 vpos = all_transforms * glm::vec4(vert_pos.x, vert_pos.y, vert_pos.z, 1.0f);
		
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
	
	BoundingBox bbox;
	
	bbox.position = glm::vec3(minx, miny, minz);
	bbox.size = glm::vec3(maxx-minx, maxy-miny, maxz-minz);	
	
	//~ printf("bouding box size : %.3f, %.3f, %.3f\n", bbox.size.x, bbox.size.y, bbox.size.z);
	
	return bbox;
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

void Object::drawPoints()
{
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

json Object::toJSON()
{
	json j;
	j["type"] = "OBJECT";
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

	if(has_generator){
		j["mesh_generator"] = mesh_generator->toJSON();
	}		
	
	std::vector<json> filters_j;
	for (int i = 0; i < meshFilters.size(); i++)
	{
		filters_j.push_back(meshFilters[i]->toJSON());
	}
	
	j["mesh_filters"] = filters_j;
	
	
	j["parent"] = -1;
	//~ if(getParent() != nullptr)
	//~ {
		//~ j["parent"] = getParent()->getID();
	//~ }
	return j;
}

void Object::fromJSON(json _j, Shader& _shader)
{
	//~ printf("OBJECT fromJSON function fired !!!! \n");
	setName(_j["name"].get<std::string>());
	

	init();
	printf("setting up transforms params \n");
	std::vector<json> trans_params_j = _j["transforms_params"];
	param_layout.fromJSON(trans_params_j);		

	
	applyTransforms();		
	
	shader = _shader;

	switch(_j["mesh_generator"]["type"].get<int>())
	{
		case SPHERE_MESH_GENERATOR :
			//~ printf("--- setting Sphere mesh generator up \n");
			setGenerator<SphereMesh>();
			generator_type = SPHERE_MESH_GENERATOR; // really crappy design , do something !!!!
			mesh_generator->need_update = true;		  	
			break;		
		case GEOSPHERE_MESH_GENERATOR :
			//~ printf("--- setting GeoSphere mesh generator up \n");
			setGenerator<GeoSphereMesh>();
			generator_type = GEOSPHERE_MESH_GENERATOR; // really crappy design , do something !!!!
			mesh_generator->need_update = true;		  	
			break;	
			
		case GRID_MESH_GENERATOR :
			//~ printf("--- setting Grid mesh generator up \n");
			setGenerator<GridMesh>();
			generator_type = GRID_MESH_GENERATOR; // really crappy design , do something !!!!
			mesh_generator->need_update = true;		  	
			break;				

		case BOX_MESH_GENERATOR :
			//~ printf("--- setting Box mesh generator up \n");
			setGenerator<BoxMesh>();
			generator_type = BOX_MESH_GENERATOR; // really crappy design , do something !!!!
			mesh_generator->need_update = true;		  	
			break;		
			
		case CYLINDER_MESH_GENERATOR:
			//~ printf("--- setting Cylinder mesh generator up \n");
			setGenerator<CylinderMesh>();
			generator_type = CYLINDER_MESH_GENERATOR; // really crappy design , do something !!!!
			mesh_generator->need_update = true;		  
			break;		
			
		case FILE_MESH_GENERATOR:
			//~ printf("--- setting File mesh generator up \n");
			setGenerator<FileMesh>();
			generator_type = FILE_MESH_GENERATOR; // really crappy design , do something !!!!
			mesh_generator->need_update = true;		  
			break;						
		default : 
			break;
		
	}
	
	std::vector<json> params_j = _j["mesh_generator"]["params"];
	mesh_generator->param_layout.fromJSON(params_j);


	
	for (int i = 0; i < _j["mesh_filters"].size(); i++)
	{
		json cur_j = _j["mesh_filters"][i];
		//~ printf("filter name is %s\n", cur_j["name"].get<std::string>().c_str());
		//~ printf("filter type is %d\n", cur_j["type"].get<int>());
		//~ printf("---------------------\n");
		
		ComputeNormalsMeshFilter * p_normals = nullptr;
		DuplicateMeshFilter * p_duplicate = nullptr;
		FromPolarMeshFilter * p_from_polar = nullptr;
		InflateMeshFilter * p_inflate = nullptr;
		MirrorMeshFilter * p_mirror = nullptr;
		SpherifyMeshFilter * p_spherify = nullptr;
		TransformMeshFilter * p_transform = nullptr;
		TwistMeshFilter * p_twist = nullptr;
		std::vector<json> json_vector = cur_j["params"];
		switch(cur_j["type"].get<int>()){
			
			case COMPUTE_NORMALS_MESH_FILTER : 
				//~ printf("COMPUTE_NORMALS_MESH_FILTER detected \n");
				p_normals = new ComputeNormalsMeshFilter();	
				p_normals->param_layout.fromJSON(json_vector);
				mesh = p_normals->applyFilter(mesh);
				
				p_normals->setName(cur_j["name"]);
				meshFilters.push_back(p_normals);		
							
				break;
			case DUPLICATE_MESH_FILTER : 
				//~ printf("DUPLICATE_MESH_FILTER detected \n");
				p_duplicate = new DuplicateMeshFilter();	
				p_duplicate->param_layout.fromJSON(json_vector);
				mesh = p_duplicate->applyFilter(mesh);
				
				p_duplicate->setName(cur_j["name"]);
				meshFilters.push_back(p_duplicate);		
								
				break;				
			case FROM_POLAR_MESH_FILTER : 
				//~ printf("FROM_POLAR_MESH_FILTER detected \n");
				p_from_polar = new FromPolarMeshFilter();	
				p_from_polar->param_layout.fromJSON(json_vector);
				mesh = p_from_polar->applyFilter(mesh);
				
				p_from_polar->setName(cur_j["name"]);
				meshFilters.push_back(p_from_polar);					
				break;	
			case INFLATE_MESH_FILTER : 
				//~ printf("INFLATE_MESH_FILTER detected \n");
				p_inflate = new InflateMeshFilter();	
				p_inflate->param_layout.fromJSON(json_vector);
				mesh = p_inflate->applyFilter(mesh);
				
				p_inflate->setName(cur_j["name"]);
				meshFilters.push_back(p_inflate);				
				break;	
			case MIRROR_MESH_FILTER : 
				//~ printf("MIRROR_MESH_FILTER detected \n");
				p_mirror = new MirrorMeshFilter();	
				p_mirror->param_layout.fromJSON(json_vector);
				mesh = p_mirror->applyFilter(mesh);
				
				p_mirror->setName(cur_j["name"]);
				meshFilters.push_back(p_mirror);						
				break;	
			case SPHERIFY_MESH_FILTER : 
				//~ printf("SPHERIFY_MESH_FILTER detected \n");
				p_spherify = new SpherifyMeshFilter();	
				p_spherify->param_layout.fromJSON(json_vector);
				mesh = p_spherify->applyFilter(mesh);
				
				p_spherify->setName(cur_j["name"]);
				meshFilters.push_back(p_spherify);		
					
				
				break;	
			case TRANSFORM_MESH_FILTER : 
				//~ printf("TRANSFORM_MESH_FILTER detected \n");
				p_transform = new TransformMeshFilter();
				
				p_transform->param_layout.fromJSON(json_vector);
				p_transform->applyFilter(mesh);
				p_transform->setName(cur_j["name"]);
				meshFilters.push_back(p_transform);
				//~ printf("\tjust applied mesh Filter \n");
				
				break;	
			case TWIST_MESH_FILTER : 
				//~ printf("TWIST_MESH_FILTER detected \n");
				p_twist = new TwistMeshFilter();	
				p_twist->param_layout.fromJSON(json_vector);
				mesh = p_twist->applyFilter(mesh);
				
				p_twist->setName(cur_j["name"]);
				meshFilters.push_back(p_twist);					
				break;	
				
			default :
				break;
				
				
		}
	}
	
	if(_j["mesh_filters"].size() > 0){
		hasFilters = true;
	}
	
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
