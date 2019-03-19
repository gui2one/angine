#ifndef MESH_FILTER_H
#define MESH_FILTER_H

#include "../pch.h"
#include "../mesh.h"
//~ #include "../param.h"

#include "../param_layout.h"


enum MESH_FILTER_TYPE{
	COMPUTE_NORMALS_MESH_FILTER,
	DUPLICATE_MESH_FILTER,
	FROM_POLAR_MESH_FILTER,
	INFLATE_MESH_FILTER,
	MIRROR_MESH_FILTER,
	SPHERIFY_MESH_FILTER,
	TRANSFORM_MESH_FILTER,
	TWIST_MESH_FILTER
};


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

		inline void setType(MESH_FILTER_TYPE _type){ type = _type;}
		inline MESH_FILTER_TYPE getType() const { return type; }
		
		ParamLayout param_layout;

		json toJSON();
		void fromJSON(json& _json);
		
		void setName(std::string str_name);
		char name[200] = {'_'};
		
	private:
		
		MESH_FILTER_TYPE type;
};

#endif // MESH_FILTER_H
