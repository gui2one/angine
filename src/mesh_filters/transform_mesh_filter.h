#ifndef TRANSFORM_MESH_FILTER_H
#define TRANSFORM_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class TransformMeshFilter: public MeshFilter
{
	public:
		TransformMeshFilter();
		TransformMeshFilter(const TransformMeshFilter& other);
		
		Mesh applyFilter(Mesh & source_mesh)override;
		
		ParamMenu * p_menu;
		ParamVec3 * p_translate;
		ParamVec3 * p_rotate;
		ParamVec3 * p_scale;
			
	private:
		
		/* add your private declarations */
};

#endif /* TRANSFORM_MESH_FILTER_H */ 
