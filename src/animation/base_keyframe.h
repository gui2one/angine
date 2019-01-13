#ifndef BASE_KEYFRAME_H
#define BASE_KEYFRAME_H

#include "../pch.h"

class BaseKeyframe
{
	public:
		BaseKeyframe();
		virtual ~BaseKeyframe();
		
		inline void setFrame(float _frame){ frame = _frame; }
		inline float setFrame(){ return frame; }
		
	private:
		float frame = 0.0;
		/* add your private declarations */
};

template<class T>
class Keyframe : public BaseKeyframe
{
	public :
		inline Keyframe<T>() : BaseKeyframe(){}
		inline void setValue(T _val){ value = _val; }
		inline T getValue(){ return value; }
		
	private:
		T value;
};

#endif /* BASE_KEYFRAME_H */ 
