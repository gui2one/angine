#include "base_keyframe.h"


BaseKeyframe::BaseKeyframe()
{
	
}

BaseKeyframe::BaseKeyframe(const BaseKeyframe& other){
	printf("%% baseKeyFrame copy constructor %%%%%%%%%%%%%%%%\n");
	frame = other.getFrame();
}

BaseKeyframe::~BaseKeyframe()
{
	
}




