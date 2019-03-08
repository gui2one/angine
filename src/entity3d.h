#ifndef ENTITY3D_H
#define ENTITY3D_H


#include "pch.h"

#include "param_layout.h"
class Entity3D
{
	public:
		Entity3D();
		// copy constructor
		Entity3D(const Entity3D& other);		
		virtual ~Entity3D();
	
		glm::mat4 transforms = glm::mat4(1.0);		
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		
		void applyTransforms();

		glm::vec3 getWorldPosition();
		inline Entity3D* getParent(){ return parent; }
		inline void setParent(Entity3D* _entity){ parent = _entity;}
		inline void resetParent(){ parent = nullptr;}
		
		std::vector<Entity3D*> getParents();
		void applyParentsMatrices(glm::mat4 & source_matrix);


		inline Entity3D* getLookAtTarget(){ return look_at_target; }
		inline void setLookAtTarget(Entity3D* _entity){ look_at_target = _entity;}
		inline void resetLookAtTarget(){ look_at_target = nullptr;}
		
		inline void setID(int _id){	id = _id;}			
		inline int getID(){	return id;}
		
		
		ParamLayout param_layout;
		
		ParamVec3 * p_pos;
		ParamVec3 * p_rot;
		ParamVec3 * p_scale;
		
		void setName(std::string str_name);
		char name[100] = {'n','e','w','_','o','b','j','e','c','t'};
		glm::vec4 color;
		
		

	private:
		Entity3D * parent 		 = nullptr;
		Entity3D * look_at_target = nullptr;
		int id;
		/* add your private declarations */
};

#endif /* ENTITY3D_H */ 
