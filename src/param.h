#ifndef PARAM2_H
#define PARAM2_H

#include "pch.h"
#include <functional>
#include <algorithm>
#include "animation/base_keyframe.h"

enum PARAMTYPE{
	PARAM_DEFAULT,
	PARAM_FLOAT,
	PARAM_VEC3,
	PARAM_STRING,
	PARAM_INT,
	PARAM_BOOL,
	PARAM_ACTION,
	PARAM_MENU
};

enum INTERPOLATION_TYPE{
	LINEAR,
	SMOOTHSTEP
};

class BaseParam{
	public:
	
		inline BaseParam(){}
		BaseParam(const BaseParam& other);
		inline virtual ~BaseParam(){}
		inline void setType(PARAMTYPE _type){ type = _type; }
		inline PARAMTYPE getType() const { return type; }

		inline void setInterpolationType(INTERPOLATION_TYPE _type){ interpolation_type = _type; }
		inline INTERPOLATION_TYPE getInterpolationType() const { return interpolation_type; }
		
		float lerpf(float _a, float _b, float _pos);
		inline void setName(std::string _name){ name = _name; }
		inline std::string getName() const { return name; }		
	
		/////
		// keyframes
		//////
		std::vector<BaseKeyframe *> keyframes;
		
		
		inline std::vector<BaseKeyframe *> getKeyframes(){ return keyframes; }
		BaseKeyframe * getKeyframe(int _frame);
		
		
		
		inline void setKeyframes(std::vector<BaseKeyframe *> _keys){ keyframes = _keys; }
		inline unsigned int getNumKeyframes(){ return (unsigned int)keyframes.size(); }
		inline void removeAllKeyframes(){ keyframes.clear(); };
		
		void removeKeyframeAtFrame( int _frame);
		
		void addKeyframe(BaseKeyframe * _key);
		bool isKeyframe(int _frame);
		
		virtual json toJSON();
		void fromJSON(json & _json);
	private:	
	
		std::string name;
		PARAMTYPE type = PARAM_DEFAULT;
		INTERPOLATION_TYPE interpolation_type = LINEAR;
		
		
};

class ParamFloat : public BaseParam
{
	public:
	
		//~ ParamFloat();
		inline ParamFloat(std::string _name, float _val = 0.6): BaseParam(){
			
			setType(PARAM_FLOAT); 
			setName(_name);
			value = _val;
			
		}
		
		ParamFloat( const ParamFloat& other);
		

		
		float value;
		inline void setValue(float _val){ value = _val;}
		inline float getValue(){ return value;}
		Keyframe<float>* getKeyframeAtFrame(float _frame);
		float getValueAtFrame(int _frame);
		
		
		
		
	private:

};

class ParamInt : public BaseParam
{
	public:
	
		
		inline ParamInt(std::string _name, int _val = 0, int _min = -1000000, int _max = 1000000): BaseParam(){
			
			setType(PARAM_INT); 
			setName(_name);
			value = _val;
			min = _min;
			max = _max;
			//~ printf("creating a Int type parameter ....\n");
			//~ printf("\tname : %s -- value : %d\n", getName().c_str(), value);
		}
		

		int value;
		int min, max;
		inline void setValue(int _val){ 
			if(_val < min)				
				value = min;
			else if(_val > max)
				value = max;
			else 
				value = _val;
				
		}
		inline int getValue(){ return value;}
		
		
	private:

};

class ParamString : public BaseParam
{
	public:
		inline ParamString(std::string _name, std::string _val) : BaseParam(){
			
			setType(PARAM_STRING); 
			setName(_name);
			value = _val;
			//~ printf("creating a String type parameter ....\n");
			//~ printf("\tname : %s -- value : %s\n", getName().c_str(), value.c_str());
		}
		

		std::string value;
		inline void setValue(std::string _val){ value = _val;}
		inline std::string getValue(){ return value;}
};

class ParamBool : public BaseParam
{
	public:
		inline ParamBool(std::string _name, bool _val) : BaseParam(){
			
			setType(PARAM_BOOL); 
			setName(_name);
			value = _val;
			//~ printf("creating a Boolean type parameter ....\n");
			//~ printf("\tname : %s -- value : %d\n", getName().c_str(), value);
		}
		

		bool value;
		inline void setValue(bool _val){ value = _val;}
		inline bool getValue(){ return value;}
};

class ParamVec3 : public BaseParam
{
	public:
		inline ParamVec3(std::string _name, glm::vec3 _val, std::string _prefix = "t_"): BaseParam(){
			setType(PARAM_VEC3); 
			setName(_name);
			value = _val;
			prefix = _prefix;
			
			std::string name_x = _prefix ;
			name_x += "_x";
			param_x = new ParamFloat(name_x, _val.x);
			
			std::string name_y = _prefix ;
			name_y += "_y";
			param_y = new ParamFloat(name_y, _val.y);

			std::string name_z = _prefix ;
			name_z += "_z";
			param_z = new ParamFloat(name_z, _val.z);
			//~ printf("creating a Action type parameter ....\n");
			//~ printf("\tname : %s -- value : --\n", getName().c_str());			
		}
		
		ParamVec3(const ParamVec3& other);
		
		
		glm::vec3 value;
		
		ParamFloat * param_x;
		ParamFloat * param_y;
		ParamFloat * param_z;
		std::string prefix;
		inline void setValue(glm::vec3 _val){ 
			
			param_x->setValue(_val.x);
			param_y->setValue(_val.y);
			param_z->setValue(_val.z);
			
			value = _val;
			
		}
		inline glm::vec3 getValue(){ return glm::vec3(param_x->getValue(), param_y->getValue(), param_z->getValue());}		
		
	
};

class ParamAction : public BaseParam
{
	public:
		inline ParamAction(std::string _name, std::function<void()> _val): BaseParam(){
			setType(PARAM_ACTION); 
			setName(_name);
			value = _val;
			//~ printf("creating a Action type parameter ....\n");
			//~ printf("\tname : %s -- value : --\n", getName().c_str());			
		}
		
		std::function<void()> value;
		
		inline void setValue(std::function<void()> _val){ value = _val;}
		inline std::function<void()> getValue(){ return value;}		
		
	
};

class ParamMenu : public BaseParam
{
	public :
		inline ParamMenu(std::string _name, std::vector<std::string> _value): BaseParam(){
			setType(PARAM_MENU);
			setName(_name);
			value = _value;
		}
	
		std::vector<std::string> value;
		int current_choice = 0;
		inline void setValue(std::vector<std::string> _val){ value = _val;}
		inline std::vector<std::string> getValue(){ return value;}		
};

#endif /* PARAM2_H */ 
