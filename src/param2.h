#ifndef PARAM2_H
#define PARAM2_H

#include "pch.h"
#include <functional>

enum PARAMTYPE{
	PARAM_DEFAULT,
	PARAM_FLOAT,
	PARAM_STRING,
	PARAM_INT,
	PARAM_BOOL,
	PARAM_ACTION
};

class BaseParam{
	public:
	
		inline BaseParam(){}
		inline void setType(PARAMTYPE _type){ type = _type; }
		inline PARAMTYPE getType(){ return type; }

		
		inline void setName(std::string _name){ name = _name; }
		inline std::string getName(){ return name; }		
	
		inline virtual ~BaseParam(){}

	private:	
	
		std::string name;
		PARAMTYPE type = PARAM_DEFAULT;
		
};

class ParamFloat : public BaseParam
{
	public:
	
		//~ ParamFloat();
		inline ParamFloat(std::string _name, float _val = 0.6): BaseParam(){
			
			setType(PARAM_FLOAT); 
			setName(_name);
			value = _val;
			printf("creating a float type parameter ....\n");
			printf("\tname : %s -- value : %.3f\n", getName().c_str(), value);
		}
		

		float value;
		inline void setValue(float _val){ value = _val;}
		inline float getValue(){ return value;}
		
		
	private:

};

class ParamInt : public BaseParam
{
	public:
	
		
		inline ParamInt(std::string _name, int _val = 0): BaseParam(){
			
			setType(PARAM_INT); 
			setName(_name);
			value = _val;
			printf("creating a Int type parameter ....\n");
			printf("\tname : %s -- value : %d\n", getName().c_str(), value);
		}
		

		int value;
		inline void setValue(int _val){ value = _val;}
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
			printf("creating a String type parameter ....\n");
			printf("\tname : %s -- value : %s\n", getName().c_str(), value.c_str());
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
			printf("creating a Boolean type parameter ....\n");
			printf("\tname : %s -- value : %d\n", getName().c_str(), value);
		}
		

		bool value;
		inline void setValue(bool _val){ value = _val;}
		inline bool getValue(){ return value;}
};

class ParamAction : public BaseParam
{
	public:
		inline ParamAction(std::string _name, std::function<void()> _val): BaseParam(){
			setType(PARAM_ACTION); 
			setName(_name);
			value = _val;
			printf("creating a Action type parameter ....\n");
			printf("\tname : %s -- value : --\n", getName().c_str());			
		}
		
		std::function<void()> value;
		
		inline void setValue(std::function<void()> _val){ value = _val;}
		inline std::function<void()> getValue(){ return value;}		
		
	
};
#endif /* PARAM2_H */ 
