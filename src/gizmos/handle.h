#ifndef HANDLE_H
#define HANDLE_H

#include "../pch.h"
#include "../param.h"

class Handle
{
	public:
		Handle();
		
		inline void setName(std::string _name) 
		{
			name = _name;
		}
		
		inline std::string getName() const
		{
			return name;
		}
			
	private:
		BaseParam* target_param;
		std::string name;
		/* add your private declarations */
};

#endif /* HANDLE_H */ 
