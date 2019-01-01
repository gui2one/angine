#include "file_mesh.h"


FileMesh::FileMesh()
{

	Param<std::string> file_path_param{"path ", file_path.c_str()};	
	paramsString.push_back(file_path_param);
	
	
	Param<std::function<void()>> loadAction{"load file", [this](){ 
		printf("---- %s , %s \n", "hello", this->file_path.c_str());
		
	}};
	
	paramsAction.push_back(loadAction);
}

Mesh FileMesh::generate(){
	Mesh mesh;
	
	ObjLoader loader;
	mesh = loader.assimp_load(paramsString[0].value);
	
	mesh_cache = mesh;
	return mesh;
}

FileMesh::~FileMesh()
{
	
}

