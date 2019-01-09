#ifndef DUPLICATE_MESH_FILTER_H
#define DUPLICATE_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class DuplicateMeshFilter: public MeshFilter
{
	public:
		DuplicateMeshFilter();
		
		Mesh applyFilter(Mesh & source_mesh)override;
		
		
		ParamInt  * p_num;
		ParamMenu * p_menu;
		ParamVec3 * p_translate;
		ParamVec3 * p_rotate;
		ParamVec3 * p_scale;
			
	private:
		
		/* add your private declarations */
};

#endif /* DUPLICATE_MESH_FILTER_H */ 
