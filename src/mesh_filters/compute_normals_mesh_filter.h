#ifndef COMPUTE_NORMALS_MESH_FILTER_H
#define COMPUTE_NORMALS_MESH_FILTER_H

#include "../pch.h"

// for std::find
#include <algorithm>
#include <iterator>
//
#include "mesh_filter.h"

class ComputeNormalsMeshFilter: public MeshFilter
{
	public:
		ComputeNormalsMeshFilter();
		ComputeNormalsMeshFilter(const ComputeNormalsMeshFilter& other);
		
		Mesh applyFilter(Mesh & source_mesh)override;		
			
		ParamBool *p_invert;
		
	private:
		
		/* add your private declarations */
};

#endif /* COMPUTE_NORMALS_MESH_FILTER_H */ 
