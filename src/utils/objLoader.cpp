#include "objLoader.h"


ObjLoader::ObjLoader(){
	std::cout << "ObjLoader initiatied ----\n";
}

ObjLoader::~ObjLoader(){
	std::cout << "ObjLoader DELETED ----\n";
}

// unused now, but handy function
//~ std::vector<std::string> split(const std::string& str, std::string delimiter = " "){
	//~ 
	//~ std::vector<std::string> tokens;
	//~ std::string tmp = str;
	//~ std::string::size_type start = 0;
	//~ while(true ){
		//~ std::string::size_type found = tmp.find_first_of(delimiter);
		//~ if( found == std::string::npos){
			//~ //std::cout << " reached end ------------------------------- " << std::endl;	
			//~ tokens.push_back(tmp);
			//~ break;
		//~ }else{
			//~ std::string cut = tmp.substr(0, found);
			//~ tokens.push_back(cut);
			//~ tmp = tmp.substr(found+1, tmp.size() - (found-1));
			//~ //std::cout << tmp << std::endl;
			//~ start = found;
		//~ }
	//~ }
	//~ 
	//~ //std::cout << " ------------------------------- " << std::endl;	
	//~ 
	//~ return tokens;
//~ }


Mesh ObjLoader::assimp_load(std::string file_path){
	const struct aiScene* scene = NULL;
	scene = aiImportFile(file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	if(scene){
		
		std::cout << "ObjLoader assimp loading V2 : " << file_path << "\n";
		std::cout << "----------------------------\n";
		
		
		std::vector<Vertex> vertices;
		for (int i = 0; i < scene->mMeshes[0]->mNumVertices; i++){
			Vertex vertex;
			vertex.position.x = scene->mMeshes[0]->mVertices[i].x;
			vertex.position.y = scene->mMeshes[0]->mVertices[i].y;
			vertex.position.z = scene->mMeshes[0]->mVertices[i].z;
			
			vertex.normal.x = scene->mMeshes[0]->mNormals[i].x;
			vertex.normal.y = scene->mMeshes[0]->mNormals[i].y;
			vertex.normal.z = scene->mMeshes[0]->mNormals[i].z;
			
			if(scene->mMeshes[0]->HasTextureCoords(0)){
				
				vertex.t_coords.x = scene->mMeshes[0]->mTextureCoords[0][i].x;
				vertex.t_coords.y = scene->mMeshes[0]->mTextureCoords[0][i].y;
			}else{
				vertex.t_coords.x = 0.0;
				vertex.t_coords.y = 0.0;
			}
			vertices.push_back(vertex);
			//~ std::cout<< "Vertex " <<  "\n";
			//~ std::cout << "\tX: " << vertex.position.x << ", Y: " << vertex.position.y << ", Z: " << vertex.position.z <<"\n";
		
		}
		
		std::vector<unsigned int> indices;
		for (int i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
		{
			//~ std::cout << "Face : "<< i << "\n";
			for (int j = 0; j < scene->mMeshes[0]->mFaces[i].mNumIndices; j++)
			{
				unsigned int index = scene->mMeshes[0]->mFaces[i].mIndices[j];
				indices.push_back(index);
				
				
				//~ std::cout << "\tindex : "<< j << " | " << index << "\n";
			}
		}
		
		
		Mesh mesh;
		mesh.vertices = vertices;
		mesh.indices = indices;
		
		
		return mesh;
	}
	std::cout<< "problem loading " << file_path << "\n";
	Mesh empty_mesh;
	return empty_mesh;
}
