#ifndef MESH_FILTER_H
#define MESH_FILTER_H

#include "../pch.h"
#include "../mesh.h"
#include "../param.h"
//~ template<typename T>
//~ struct MeshFilterParam{
	//~ std::string name;
	//~ T value;
	//~ 
//~ };

class MeshFilter{
	
	public :
		MeshFilter();
		
		inline virtual Mesh applyFilter(Mesh & mesh){};
		
		Mesh mesh_cache;
		
		bool need_update = false;
		bool is_active = true;
		
		inline void setName(std::string str){ name = str; }
		inline std::string getName(){ return name; }
		
		
		std::vector<Param<int>> paramsInt;
		std::vector<Param<float>> paramsFloat;
		std::vector<Param<glm::vec3>> paramsVec3;
		
		
	private:
		std::string name = "Default filter name";
		
};

#endif // MESH_FILTER_H
