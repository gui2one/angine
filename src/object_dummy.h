#ifndef OBJECT_DUMMY_H
#define OBJECT_DUMMY_H

#include "pch.h"
#include "mesh.h"
#include "entity3d.h"

#include "vendor/nlohmann/json.hpp"

// for convenience
using json = nlohmann::json;
////

class ObjectDummy: public Entity3D
{
	public:
		ObjectDummy();
		ObjectDummy(const ObjectDummy & other);
		void init();
		void buildVbo();
		void draw();
		
		unsigned int m_vbo;
		
		json toJSON();
		void fromJSON(json& _json);
			
	private:
		/* add your private declarations */
};

#endif /* OBJECT_DUMMY_H */ 
