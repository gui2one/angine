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
		
		inline Entity3D* getParent(){ 
			
			return parent; 
		}
		inline void setParent(Entity3D* _entity){
			parent = _entity;
		}
		inline void resetParent(){
			parent = nullptr;
		}
		
		std::vector<Entity3D*> getParents();
		
		inline void setID(int _id){	id = _id;}			
		inline int getID(){	return id;}
		
		char name[100] = {'n','e','w','_','o','b','j','e','c','t'};
	private:
		Entity3D* parent;
		int id;
		/* add your private declarations */
};

#endif /* ENTITY3D_H */ 
