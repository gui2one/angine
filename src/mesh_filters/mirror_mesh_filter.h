#ifndef MIRROR_MESH_FILTER_H
#define MIRROR_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class MirrorMeshFilter: public MeshFilter
{
	public:
		MirrorMeshFilter();
		
		Mesh applyFilter(Mesh & source_mesh)override;
		
		
		ParamVec3  * p_center_pos;
		ParamVec3  * p_axis;
		ParamFloat * p_distance;
		
			
	private:
		
		/* add your private declarations */
};

#endif /* MIRROR_MESH_FILTER_H */ 
