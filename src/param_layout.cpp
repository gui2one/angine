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
			vec3_j[p_vec3->getName()] = {
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
		printf("%s \n", params[i]->getName().c_str());
		printf("is int ? -- %s\n", params_j[i].is_number() == true ? "true": "false");
		
		ParamFloat * p_float = nullptr;
		ParamInt * p_int = nullptr;
		ParamVec3 * p_vec3 = nullptr;
		
		printf("Param type is --> %d\n", params_j[i]["type"].get<int>());
		
		switch(params_j[i]["type"].get<int>()) {
			case PARAM_FLOAT :
				
				
				if( p_float = dynamic_cast<ParamFloat*>(params[i]))
				{
					try {
						std::vector<json> keys_j = params_j[i].at("keyframes");
						
						printf("num Keyframes is == %d \n", keys_j.size());
						
						p_float->keyframes.clear();
						p_float->setInterpolationType((INTERPOLATION_TYPE)params_j[i].at("keyframes_interpolation").get<int>());
						for (int k = 0; k < keys_j.size(); k++)
						{
							Keyframe<float> * new_key = new Keyframe<float>();
							new_key->setFrame(keys_j[k][0].get<float>());
							
							printf("key value is --> %f \n", keys_j[k][1].get<float>());
							new_key->setValue((float)(keys_j[k][1].get<float>()));
							p_float->keyframes.push_back( new_key );
						}
						
						
							
						
						
					}catch (nlohmann::detail::out_of_range e){
						printf("there was an execption thrown \n");
						printf("trying to set value directly !!!!!! \n");
						p_float->value = params_j[i]["value"].get<float>();
					}
					printf("this is a ParamFloat \n");

				}
				
				break;
			case PARAM_INT :			
				printf("this is a ParamInt \n");
				
				if( p_int = dynamic_cast<ParamInt*>(params[i]))
				{
					try {
						std::vector<json> keys_j = params_j[i].at("keyframes");
						
						printf("num Keyframes is == %d \n", keys_j.size());
						
						p_int->keyframes.clear();
						p_int->setInterpolationType((INTERPOLATION_TYPE)params_j[i].at("keyframes_interpolation").get<int>());
						for (int k = 0; k < keys_j.size(); k++)
						{
							Keyframe<int> * new_key = new Keyframe<int>();
							new_key->setFrame(keys_j[k][0].get<float>());
							
							printf("key value is --> %f \n", keys_j[k][1].get<int>());
							new_key->setValue((int)(keys_j[k][1].get<int>()));
							p_int->keyframes.push_back( new_key );
						}
						
						
							
						
						
					}catch (nlohmann::detail::out_of_range e){
						printf("there was an execption thrown \n");
						printf("trying to set value directly !!!!!! \n");
						p_int->value = params_j[i]["value"].get<int>();
					}	
					
				}			
				
				break;
				
			case PARAM_VEC3 :
				if( p_vec3 = dynamic_cast<ParamVec3*>(params[i]))
				{
					try {
						std::vector<json> keys_j = params_j[i].at("keyframes");
						
						printf("num Keyframes is == %d \n", keys_j.size());
						
						p_vec3->keyframes.clear();
						p_vec3->setInterpolationType((INTERPOLATION_TYPE)params_j[i].at("keyframes_interpolation").get<int>());
						for (int k = 0; k < keys_j.size(); k++)
						{
							Keyframe<int> * new_key = new Keyframe<int>();
							new_key->setFrame(keys_j[k][0].get<float>());
							
							printf("key value is --> %f \n", keys_j[k][1].get<int>());
							new_key->setValue((int)(keys_j[k][1].get<int>()));
							p_vec3->keyframes.push_back( new_key );
						}
						
						
							
						
						
					}catch (nlohmann::detail::out_of_range e){
						printf("there was an execption thrown for a VEC3\n");
						printf("trying to set value directly !!!!!! \n");
						p_vec3->setValue(glm::vec3(
							params_j[i]["value"][0].get<float>(),
							params_j[i]["value"][1].get<float>(),
							params_j[i]["value"][2].get<float>()
						));
					}	
					
				}				
				break;
			default :
				printf("default case \n");
				break;
			
		}
		
	}	
}

ParamLayout::~ParamLayout(){


}


