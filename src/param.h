#ifndef PARAM_H
#define PARAM_H

#include "pch.h"

class ParamInterface{
	
	public : 
		
		//~ static auto value;
		
		
};

template<typename T>
class Param //: public ParamInterface
{
	public:
		//~ Param();
		std::string name;
		T value;
		
			
	private:
		/* add your private declarations */
};

#endif /* PARAM_H */ 
