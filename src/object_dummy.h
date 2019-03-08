#ifndef OBJECT_DUMMY_H
#define OBJECT_DUMMY_H

#include "pch.h"
#include "mesh.h"
#include "entity3d.h"

class ObjectDummy: public Entity3D
{
	public:
		ObjectDummy();
		ObjectDummy(const ObjectDummy & other);
		void init();
		void buildVbo();
		void draw();
		
		unsigned int m_vbo;
			
	private:
		/* add your private declarations */
};

#endif /* OBJECT_DUMMY_H */ 
