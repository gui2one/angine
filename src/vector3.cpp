#include "vector3.h"

std::string Vector3::toString(){

	
	std::ostringstream ss;
	ss << "Vector3 : "<<this->x<<", "<< this->y<< ", " << this->z;
	std::string str(ss.str());	

	
	return str;
}

