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
		
		Mesh applyFilter(Mesh & source_mesh)override;		
			
	private:
		std::string name = "compute_normals";
		/* add your private declarations */
};

#endif /* COMPUTE_NORMALS_MESH_FILTER_H */ 
