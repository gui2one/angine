#include "param_layout.h"

ParamLayout::ParamLayout()
{

}

ParamLayout::ParamLayout(const ParamLayout& other){
	printf("ParamLayout COPY CONSTRUCTOR\n");
	std::vector<BaseParam*> params;
	for (int i = 0; i < other.getSize(); i++)
	{
		//~ BaseParam * p = other.getParam(i);
		ParamFloat * p_float = nullptr;
		ParamInt * p_int = nullptr;
		ParamVec3 * p_vec3 = nullptr;
		
		if( p_float = dynamic_cast<ParamFloat*>(other.getParam(i))){
			printf("---- COPYING paramFloat type --------------\n");
			ParamFloat * param = new ParamFloat((const ParamFloat&)*p_float);
			params.push_back(param);			
		}else if( p_vec3 = dynamic_cast<ParamVec3*>(other.getParam(i))){
			printf("---- COPYING ParamVec3 type --------------\n");
			ParamVec3 * param = new ParamVec3((const ParamVec3&)*p_vec3);
			params.push_back(param);			
		}

	}
	
	//~ params = param_layout.params;
}

void ParamLayout::push(BaseParam* _param)
{
	params.push_back(_param);
}


BaseParam* ParamLayout::getParam(size_t i) const
{
	return params[i];
}

ParamLayout::~ParamLayout(){


}


