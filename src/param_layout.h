#ifndef PARAM_LAYOUT_H
#define PARAM_LAYOUT_H

#include "pch.h"

#include "param.h"

#include <functional>


class ParamLayout
{
	public:
		ParamLayout();	
		~ParamLayout();
		void push(BaseParam* _param);
		BaseParam* getParam(size_t i);
			
		inline size_t getSize(){ return params.size();}
	private:
	
		std::vector<BaseParam *> params;
		/* add your private declarations */
};

#endif /* PARAM_LAYOUT_H */ 
