#ifndef SPHERIFY_MESH_FILTER_H
#define SPHERIFY_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class SpherifyMeshFilter: public MeshFilter
{
	public:
		SpherifyMeshFilter();
		
		Mesh applyFilter(Mesh & source_mesh)override;
		
		ParamFloat* p_radius;	
		
	private:
		
		/* add your private declarations */
};

#endif /* SPHERIFY_MESH_FILTER_H */ 
