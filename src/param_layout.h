#ifndef PARAM_LAYOUT_H
#define PARAM_LAYOUT_H

#include "pch.h"

#include "param.h"

#include <functional>


class ParamLayout
{
	public:
		ParamLayout();	
		ParamLayout(const ParamLayout& other);
		~ParamLayout();
		void push(BaseParam* _param);
		BaseParam* getParam(size_t i) const;
		inline std::vector<BaseParam *> getParams() { return params;}
		inline void clearParams(){ params.clear();}
		inline size_t getSize() const{ return params.size();}
	private:
	
		std::vector<BaseParam *> params;
		/* add your private declarations */
};

#endif /* PARAM_LAYOUT_H */ 
