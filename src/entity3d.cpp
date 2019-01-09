#include "entity3d.h"


Entity3D::Entity3D()
{
	
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

Entity3D::~Entity3D()
{
	
}

