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
			if((float)_frame >= first_keyframe_frame)
			{
				for (int key_id = 0; key_id < keyframes.size()-1; key_id++)
				{
					if(keyframes[key_id+1]->getFrame() > _frame)
					{
						if(!before_found)
						{
							before_key = keyframes[key_id];
							before_found = true;
						}
					}
				}
			}
			
			bool after_found = false;
			if((float)_frame <= last_keyframe_frame)
			{
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
			}							
			if(before_key != nullptr && after_key != nullptr){

				float before_frame = before_key->getFrame();
				float before_value = dynamic_cast<Keyframe<float>*>(before_key)->getValue();
				float after_frame = after_key->getFrame();
				float after_value = dynamic_cast<Keyframe<float>*>(after_key)->getValue();								

				//actual interpolation !!!
				float ratio = (_frame - before_frame) / (after_frame - before_frame);

				float _val = before_value + (after_value - before_value)*ratio;
				return _val;

				
			}else if(before_key != nullptr && after_key == nullptr){
				// get the value of the first key
				float _val = dynamic_cast<Keyframe<float>*>(keyframes[0])->getValue();
				return _val;
			}else if(before_key == nullptr && after_key != nullptr){
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



