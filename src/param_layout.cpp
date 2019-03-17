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


std::vector<json> ParamLayout::toJSON()
{

	json j;
	std::vector<json> all_params;
	for (int i = 0; i < getSize(); i++)
	{
		BaseParam * p_parm = getParam(i);
		ParamFloat * p_float = nullptr;
		ParamInt * p_int = nullptr;
		ParamVec3 * p_vec3 = nullptr;

		if(p_float = dynamic_cast<ParamFloat*>(p_parm)){

			all_params.push_back(p_float->toJSON());
		}else if(p_int = dynamic_cast<ParamInt*>(p_parm)){
			
			all_params.push_back(p_int->toJSON());
		}else if(p_vec3 = dynamic_cast<ParamVec3*>(p_parm)){
			
			json vec3_j;
			vec3_j["type"] = PARAM_VEC3;
			vec3_j["name"] = p_vec3->getName();
			vec3_j["value"] = {
				p_vec3->param_x->toJSON(),
				p_vec3->param_y->toJSON(),
				p_vec3->param_z->toJSON()
			};
			all_params.push_back(vec3_j);
			//~ all_params.push_back(p_vec3->param_x->toJSON());
			//~ all_params.push_back(p_vec3->param_y->toJSON());
			//~ all_params.push_back(p_vec3->param_z->toJSON());
		}

		//~ printf("param to JSON\n");
	}
	
	
	
	return all_params;
}

void ParamLayout::fromJSON(std::vector<json>& _json_vector)
{
	std::vector<json> params_j = _json_vector;
	std::vector<BaseParam*> params = getParams();
	
	for (int i = 0; i < params.size(); i++)
	{
		params[i]->fromJSON( params_j[i]);

	}	
}

ParamLayout::~ParamLayout(){


}


