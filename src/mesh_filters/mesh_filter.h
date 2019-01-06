#ifndef MESH_FILTER_H
#define MESH_FILTER_H

#include "../pch.h"
#include "../mesh.h"
#include "../param.h"

#include "../param_layout.h"


class MeshFilter{
	
	public :
		MeshFilter();
		
		inline virtual Mesh applyFilter(Mesh & mesh){};
		inline virtual ~MeshFilter(){
			for (size_t i = 0; i < param_layout.getSize(); i++)
			{
				delete param_layout.getParam(i);
			}
			
			printf("Just deleted param Layout content\n");			
		}		
		Mesh mesh_cache;
		
		bool need_update = false;
		bool is_active = true;
		
		//~ inline void setName(char * _ch){ name = _ch; }
		//~ inline char* getName(){ return name; }
		
		
		ParamLayout param_layout;
		
		std::vector<Param<int>> paramsInt;
		std::vector<Param<float>> paramsFloat;
		std::vector<Param<glm::vec3>> paramsVec3;
		std::vector<Param<bool>> paramsBool;
		
		char name[200] = {'r', 'u'};
		
	private:
		
		
};

#endif // MESH_FILTER_H
