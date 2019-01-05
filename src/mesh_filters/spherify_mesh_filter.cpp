#include "spherify_mesh_filter.h"


SpherifyMeshFilter::SpherifyMeshFilter()
	: MeshFilter()
{
	Param<float> param1{"radius",0.5};
	paramsFloat.push_back(param1);

}


