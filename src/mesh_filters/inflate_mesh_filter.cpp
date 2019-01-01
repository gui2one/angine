#include "inflate_mesh_filter.h"


InflateMeshFilter::InflateMeshFilter()
	: MeshFilter()
{
	Param<float> param1{"amount",0.0};
	paramsFloat.push_back(param1);

}


