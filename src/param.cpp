#include "param.h"

//~ ParamFloat::ParamFloat(){
	//~ printf("Param float empty constructor\n");
//~ }

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
			
			printf("just delete keyframe %d \n", _frame);
			
			
		}
	}
}

float BaseParam::lerpf( float _a, float _b, float _pos){

	float _val = _a + (_b - _a)*_pos;
	return _val;

	
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
  return x * x * (3 - 2 * x);
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
					
					return smoothstep(before_value, after_value, ratio);
					printf("I have A SMOOTHSTEP interpolation type\n");
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



