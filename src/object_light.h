#ifndef OBJECT_LIGHT_H
#define OBJECT_LIGHT_H

#include "pch.h"
#include "entity3d.h"

class ObjectLight: public Entity3D
{
	public:
		ObjectLight();
		virtual ~ObjectLight();
		
		void init();
	
	private:
		/* add your private declarations */
};

#endif /* OBJECT_LIGHT_H */ 
