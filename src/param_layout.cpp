#include "param_layout.h"

ParamLayout::ParamLayout()
{

}

void ParamLayout::push(BaseParam* _param)
{
	params.push_back(_param);
}


BaseParam* ParamLayout::getParam(size_t i)
{
	return params[i];
}

ParamLayout::~ParamLayout(){


}


