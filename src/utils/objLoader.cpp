#include "objLoader.h"


// unused now, but handy function
std::vector<std::string> split(const std::string& str, std::string delimiter = " "){
	
	std::vector<std::string> tokens;
	std::string tmp = str;
	std::string::size_type start = 0;
	while(true ){
		std::string::size_type found = tmp.find_first_of(delimiter);
		if( found == std::string::npos){
			//std::cout << " reached end ------------------------------- " << std::endl;	
			tokens.push_back(tmp);
			break;
		}else{
			std::string cut = tmp.substr(0, found);
			tokens.push_back(cut);
			tmp = tmp.substr(found+1, tmp.size() - (found-1));
			//std::cout << tmp << std::endl;
			start = found;
		}
	}
	
	//std::cout << " ------------------------------- " << std::endl;	
	
	return tokens;
}

Mesh ObjLoader::assimp_load(std::string file_path){
	
	const struct aiScene* scene = NULL;
	scene = aiImportFile(file_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	
	if(scene){

		
		std::vector<Vertex> vertices;
		std::vector<float> positions;
		std::vector<float> normals;
		for (int i = 0; i < scene->mMeshes[0]->mNumFaces; i++)
		{
			Vertex vertex;

			for (int j = 0; j < scene->mMeshes[0]->mFaces[i].mNumIndices; j++)
			{
					int index = scene->mMeshes[0]->mFaces[i].mIndices[j];

					vertex.position.x = scene->mMeshes[0]->mVertices[index].x;
					vertex.position.y = scene->mMeshes[0]->mVertices[index].y;
					vertex.position.z = scene->mMeshes[0]->mVertices[index].z;
					
					vertex.normal.x = scene->mMeshes[0]->mNormals[index].x;
					vertex.normal.y = scene->mMeshes[0]->mNormals[index].y;
					vertex.normal.z = scene->mMeshes[0]->mNormals[index].z;
					
					
					
					if(scene->mMeshes[0]->HasTextureCoords(0)){
						
						//~ std::cout << scene->mMeshes[0]->mTextureCoords[0][index].x  << "-------------"<< "\n";
						vertex.t_coords.x = scene->mMeshes[0]->mTextureCoords[0][index].x;
						vertex.t_coords.y = scene->mMeshes[0]->mTextureCoords[0][index].y;
					}
					
					
					vertices.push_back(vertex);
					
					positions.push_back(scene->mMeshes[0]->mVertices[index].x);
					positions.push_back(scene->mMeshes[0]->mVertices[index].y);
					positions.push_back(scene->mMeshes[0]->mVertices[index].z);
			}
			
		}
		Mesh mesh;
		
		mesh.vertices = vertices;
		
		delete scene;
		vertices.clear();
		positions.clear();
		normals.clear();
		
		return mesh;
	}else{
		Mesh empty_mesh;
		return empty_mesh;
	}
	
	
}

