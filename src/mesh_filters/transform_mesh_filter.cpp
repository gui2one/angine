#include "transform_mesh_filter.h"


TransformMeshFilter::TransformMeshFilter()
	: MeshFilter()
{
	Param<float> param1{"tx",0.0};
	paramsFloat.push_back(param1);
		
	Param<float> param2{"ty",0.0};
	paramsFloat.push_back(param2);	
	
	Param<float> param3{"tz",0.0};
	paramsFloat.push_back(param3);
	
	Param<float> param4{"rx",0.0};
	paramsFloat.push_back(param4);
		
	Param<float> param5{"ry",0.0};
	paramsFloat.push_back(param5);	
	
	Param<float> param6{"rz",0.0};
	paramsFloat.push_back(param6);	
}


