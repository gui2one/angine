#include "entity3d.h"


Entity3D::Entity3D()
{
	position = glm::vec3(0.0f,0.0f,0.0f);
	rotation = glm::vec3(0.0f,0.0f,0.0f);
	scale = glm::vec3(1.0f,1.0f,1.0f);	
}

void Entity3D::applyTransforms(){
	
	glm::mat4 temp = glm::mat4(1.0f);
	
	temp = glm::translate(temp, position);
	temp = glm::rotate(temp, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	temp = glm::rotate(temp, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	temp = glm::rotate(temp, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	temp = glm::scale(temp , scale);
	
	transforms = temp;
}

std::vector<Entity3D*> Entity3D::getParents()
{
	std::vector<Entity3D*> parents;
	Entity3D * cur = this;
	while(cur->getParent() != nullptr)
	{
		Entity3D * p = cur->getParent();
		parents.push_back(p);
		//~ parents.insert(parents.begin(), p);
		cur = p;
	}
	
	return parents;
}


Entity3D::~Entity3D()
{
	
}

