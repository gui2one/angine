#include "file_mesh.h"


FileMesh::FileMesh()
{


	
	p_file_path = new ParamString("file_path", "/home/pi/projects/angine/src/res/obj/monkey.obj");
	param_layout.push(p_file_path);
	
	p_load_action = new ParamAction("Load File", [this](){
		this->generate();
		printf("Action !!!!\n");
	});
	
	param_layout.push(p_load_action);
}

FileMesh::FileMesh(const FileMesh& other){
	
	p_file_path = new ParamString(*other.p_file_path);
	param_layout.push(p_file_path);
	
	p_load_action = new ParamAction(*other.p_load_action);	
	param_layout.push(p_load_action);	
}
Mesh FileMesh::generate(){
	Mesh mesh;
	
	ObjLoader loader;
	mesh = loader.assimp_load(p_file_path->getValue());
	
	mesh_cache = mesh;
	return mesh;
}

FileMesh::~FileMesh()
{
	
}

