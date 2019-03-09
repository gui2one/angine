#ifndef MIRROR_MESH_FILTER_H
#define MIRROR_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class MirrorMeshFilter: public MeshFilter
{
	public:
		MirrorMeshFilter();
		MirrorMeshFilter(const MirrorMeshFilter& other);
		
		Mesh applyFilter(Mesh & source_mesh)override;
		
		ParamMenu  * p_axis_menu;
		ParamFloat * p_distance;
		
			
	private:
		
		/* add your private declarations */
};

#endif /* MIRROR_MESH_FILTER_H */ 
