#ifndef FROM_POLAR_MESH_FILTER_H
#define FROM_POLAR_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class FromPolarMeshFilter: public MeshFilter
{
	public:
		FromPolarMeshFilter();
		
		Mesh applyFilter(Mesh & source_mesh)override;
		
		ParamMenu * p_menu;
		ParamVec3 * p_translate;
		ParamVec3 * p_rotate;
		ParamVec3 * p_scale;
			
	private:
		
		/* add your private declarations */
};

#endif /* FROM_POLAR_MESH_FILTER_H */ 
