#ifndef BASE_KEYFRAME_H
#define BASE_KEYFRAME_H

#include "../pch.h"

class BaseKeyframe
{
	public:
		BaseKeyframe();
		BaseKeyframe(const BaseKeyframe& other);
		virtual ~BaseKeyframe();
		
		inline void setFrame(float _frame){ frame = _frame; }
		inline float getFrame()const{ return frame; }
		
	private:
		float frame = 0;
		/* add your private declarations */
};

template<class T>
class Keyframe : public BaseKeyframe
{
	public :
		inline Keyframe<T>() : BaseKeyframe(){}
		inline Keyframe<T>(const Keyframe<T>& other): BaseKeyframe(other){
			printf("!! Keyframe<float> copy constructor !!!!!!!!!!!!!!n");
			setValue(other.getValue());
		}
		inline void setValue(T _val){ value = _val; }
		inline T getValue() const { return value; }
		
	private:
		T value;
};

#endif /* BASE_KEYFRAME_H */ 
