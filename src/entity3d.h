#ifndef ENTITY3D_H
#define ENTITY3D_H

#include "pch.h"
class Entity3D
{
	public:
		Entity3D();
		virtual ~Entity3D();
	
		glm::mat4 transforms = glm::mat4(1.0);		
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		
		void applyTransforms();
			
	private:
		/* add your private declarations */
};

#endif /* ENTITY3D_H */ 
