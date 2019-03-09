#ifndef MESH_FILTER_H
#define MESH_FILTER_H

#include "../pch.h"
#include "../mesh.h"
//~ #include "../param.h"

#include "../param_layout.h"


class MeshFilter{
	
	public :
		MeshFilter();
		MeshFilter(const MeshFilter& other);
		virtual Mesh applyFilter(Mesh & mesh);
		inline virtual ~MeshFilter(){
			for (size_t i = 0; i < param_layout.getSize(); i++)
			{
				delete param_layout.getParam(i);
			}
					
		}		
		Mesh mesh_cache;
		
		bool need_update = false;
		bool is_active = true;

		
		
		ParamLayout param_layout;

		
		
		void setName(std::string str_name);
		char name[200] = {'_'};
		
	private:
		
		
};

#endif // MESH_FILTER_H
