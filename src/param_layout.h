#ifndef PARAM_LAYOUT_H
#define PARAM_LAYOUT_H

#include "pch.h"
#include "param.h"

#include <functional>
class ParamLayout
{
	public:
		ParamLayout();
			
			
		std::vector<Param<float>> paramsFloat;
		std::vector<Param<int>> paramsInt;
		std::vector<Param<glm::vec3>> paramsVec3;
		std::vector<Param<std::string>> paramsString;
		std::vector<Param<std::function<void()>>> paramsAction;
	private:
		/* add your private declarations */
};

#endif /* PARAM_LAYOUT_H */ 
