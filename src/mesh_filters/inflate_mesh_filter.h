#ifndef INFLATE_MESH_FILTER_H
#define INFLATE_MESH_FILTER_H

#include "../pch.h"

#include "mesh_filter.h"

class InflateMeshFilter: public MeshFilter
{
	public:
		InflateMeshFilter();
		
		Mesh applyFilter(Mesh & source_mesh)override;
		
		ParamFloat * p_amount;
			
	private:
		
		/* add your private declarations */
};

#endif /* INFLATE_MESH_FILTER_H */ 
