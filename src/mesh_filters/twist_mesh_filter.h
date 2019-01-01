#ifndef TWIST_MESH_FILTER_H
#define TWIST_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"


class TwistMeshFilter: public MeshFilter
{
	public:
		TwistMeshFilter();
		
		Mesh applyFilter(Mesh & source_mesh) override;
			
	private:
		/* add your private declarations */
};

#endif /* TWIST_MESH_FILTER_H */ 
