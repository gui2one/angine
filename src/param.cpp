#include "param.h"

//~ ParamFloat::ParamFloat(){
	//~ printf("Param float empty constructor\n");
//~ }

BaseParam::BaseParam(const BaseParam& other){
	//~ printf("BaseParam COPY CONSTRUCTOR\n");
	setName(other.getName());
	std::vector<BaseKeyframe*> keyframes;
	setInterpolationType( other.getInterpolationType());
	for (int i = 0; i < other.keyframes.size(); i++)
	{
		Keyframe<float>* p_key_float = nullptr;
		
		if(p_key_float = dynamic_cast<Keyframe<float> *>(other.keyframes[i])){
			//~ printf("||||||||||||||| this a float keyframe |||||||||||||||\n");
			Keyframe<float> * new_key = new Keyframe<float>(*p_key_float);
			addKeyframe(new_key);
		}
	}	

	
}

void BaseParam::addKeyframe(BaseKeyframe * _key)
{
	keyframes.push_back(_key);
	
	sort(keyframes.begin(), keyframes.end(), [](BaseKeyframe* key1, BaseKeyframe* key2){
		return key1->getFrame() < key2->getFrame();
	});
}

BaseKeyframe * BaseParam::getKeyframe(int _frame){
	for (int i = 0; i < keyframes.size(); i++)
	{
		if(keyframes[i]->getFrame() == _frame)
		{
			return keyframes[i];
		}
	}
	return nullptr;
	
}

bool BaseParam::isKeyframe(int _frame)
{
	std::vector<BaseKeyframe *> keys = getKeyframes();
	for (int i = 0; i < keys.size(); i++)
	{
			
		if(keys[i]->getFrame() == _frame)
		{
			return true;
		}
		
	}
	
	return false;
	
}

void BaseParam::removeKeyframeAtFrame( int _frame) {
	//~ printf("just delete keyframe %d\n", _frame);
	for(int i=0; i < keyframes.size(); i++){
		if( keyframes[i]->getFrame() == _frame) {
			
			delete keyframes[i];
			keyframes.erase( keyframes.begin() + i);
			
			//~ printf("just delete keyframe %d \n", _frame);
			
			
		}
	}
}

float BaseParam::lerpf( float _a, float _b, float _pos){

	float _val = _a + (_b - _a)*_pos;
	return _val;

	
}

json BaseParam::toJSON(){
	json j;
	std::vector<json> keys_j;
	
	if( getNumKeyframes() > 0){
		
		for (int i = 0; i < getNumKeyframes(); i++)
		{
			Keyframe<float>* p_float_key = nullptr;
			Keyframe<int>* p_int_key = nullptr;
			if( p_float_key = dynamic_cast<Keyframe<float > *>(keyframes[i])){
				
				
				keys_j.push_back({p_float_key->getFrame(), p_float_key->getValue()});
			}else if( p_int_key = dynamic_cast<Keyframe<int > *>(keyframes[i])){
				
				
				keys_j.push_back({p_int_key->getFrame(), p_int_key->getValue()});
			}
		}
	

		j["keyframes_interpolation"] = getInterpolationType();
		j["keyframes"] = keys_j;
	}
	j["name"] = name;
	j["type"] = getType();

	if( getNumKeyframes() == 0)
	{
		BaseParam * cur_param = this;
		ParamFloat * p_float = nullptr;
		ParamInt * p_int = nullptr;

		if(p_float = dynamic_cast<ParamFloat*>(cur_param)){
			j["value"] = p_float->value;
		}else if(p_int = dynamic_cast<ParamInt*>(cur_param)){
			j["value"] = p_int->value;
		}
	}
	return j;
	
}

static float clamp(float x, float lowerlimit, float upperlimit) {
  if (x < lowerlimit)
    x = lowerlimit;
  if (x > upperlimit)
    x = upperlimit;
  return x;
}
static float smoothstep(float edge0, float edge1, float x) {
  // Scale, bias and saturate x to 0..1 range
  x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0); 
  // Evaluate polynomial
  return x * x * (3.0 - 2.0 * x);
}


// COPY CONSTRUCTOR FOR ParamFloat
ParamFloat::ParamFloat( const ParamFloat& other):BaseParam(other){
	//~ printf("ParamFloat COPY CONSTRUCTOR\n");
	setValue(other.value);
}

//// ParamFloat implementation
Keyframe<float>* ParamFloat::getKeyframeAtFrame(float _frame){
	
	if(getNumKeyframes() != 0){
		
		
		std::vector<BaseKeyframe *> keys = getKeyframes();
		
		for (int i = 0; i < keys.size(); i++)
		{
			Keyframe<float> * key_float = nullptr;
			if(key_float = dynamic_cast<Keyframe<float>*>(keys[i]))
			{
				if(key_float->getFrame() == _frame)
				{
					return key_float;
				}
			}
		}
		return nullptr;
	}		
	return nullptr;
	
}

float ParamFloat::getValueAtFrame(int _frame){ 
	
	
	
	bool has_keys = getNumKeyframes() > 0;
	if(has_keys)
	{
	
		if(getNumKeyframes() > 1 && isKeyframe(_frame)){
			//~ printf("on a key !!!!\n");
			Keyframe<float>* cur_key = getKeyframeAtFrame(_frame);
			return cur_key->getValue();
		}
		// there is keyframes, make interpolation
		else if( getNumKeyframes() > 1){
			
			//interpolate between nearest key before and after, if any.
			BaseKeyframe * before_key = nullptr;
			BaseKeyframe * after_key  = nullptr;
			
			float first_keyframe_frame = keyframes[0]->getFrame();
			
			
			float last_keyframe_frame = keyframes[keyframes.size()-1]->getFrame();
			

			
			
			bool before_found = false;

			for (int key_id = 0; key_id < keyframes.size()-1; key_id++)
			{
				if(keyframes[key_id+1]->getFrame() > _frame)
				{
					if(!before_found)
					{
						//~ printf("FOUND BEFORE KEY\n");
						before_key = keyframes[key_id];
						before_found = true;
					}
				}
			}
	
			
			bool after_found = false;

			for (int key_id = keyframes.size(); key_id >= 1; key_id--)
			{
				if(keyframes[key_id-1]->getFrame() < _frame)
				{
					if(!after_found)
					{
						after_key = keyframes[key_id];
						after_found = true;
					}
				}
			}
						
			if(before_key != nullptr && after_key != nullptr){
				//~ printf("between keyframes\n");
				float before_frame = before_key->getFrame();
				float before_value = dynamic_cast<Keyframe<float>*>(before_key)->getValue();
				float after_frame = after_key->getFrame();
				float after_value = dynamic_cast<Keyframe<float>*>(after_key)->getValue();								

				//actual interpolation !!!
				float ratio = (_frame - before_frame) / (after_frame - before_frame);
				if(getInterpolationType() == LINEAR)
				{
					
					return lerpf(before_value, after_value, ratio);
					
				}else if(getInterpolationType() == SMOOTHSTEP)
				{
					//~ printf("I have A SMOOTHSTEP interpolation type\n");
					//~ printf("\tValues -> %.4f, %.4f, %.4f\n", before_value, after_value, ratio);
					return smoothstep(0.0, 1.0, ratio) * (after_value - before_value) + before_value;
					
				}
				
			}else if(before_key != nullptr){
				// get the value of the first key
				//~ printf("get last key value ?\n");
				float _val = dynamic_cast<Keyframe<float>*>(keyframes[0])->getValue();
				return _val;
			}else if(after_key != nullptr){
				
				// get the value of the last key
				
				
				float _val = dynamic_cast<Keyframe<float>*>(keyframes[getNumKeyframes()-1])->getValue();
				return _val;
			}
			
			
			
		}else{
			// there's only one keyframe, get that value
			float _val = dynamic_cast<Keyframe<float>*>(keyframes[0])->getValue();
			return _val;
		}
		//// old callback
		//ptr_object->mesh_generator->need_update = true;
	}	
	return value;
	
}


ParamVec3::ParamVec3(const ParamVec3& other):BaseParam(other){
	//~ printf("ParamVec3 COPY CONSTRUCTOR\n");
	//~ setName(other.getName());
	param_x = new ParamFloat(*other.param_x);
	param_y = new ParamFloat(*other.param_y);
	param_z = new ParamFloat(*other.param_z);
}

