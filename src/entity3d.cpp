#include "entity3d.h"


Entity3D::Entity3D()
{
	position = glm::vec3(0.0f,0.0f,0.0f);
	rotation = glm::vec3(0.0f,0.0f,0.0f);
	scale = glm::vec3(1.0f,1.0f,1.0f);	
	
	p_pos = new ParamVec3("position", position);
	param_layout.push(p_pos);
	p_rot = new ParamVec3("rotation", rotation);
	param_layout.push(p_rot);
	p_scale = new ParamVec3("scale", scale);
	param_layout.push(p_scale);
}

void Entity3D::setName(std::string str_name)
{

	
	
	for (int i = 0; i < str_name.size(); i++)
	{
		name[i] = str_name[i];
	}
	
}

void Entity3D::applyTransforms(){
	
	glm::mat4 temp = glm::mat4(1.0f);
	
	temp = glm::translate(temp, p_pos->getValue());
	temp = glm::rotate(temp, glm::radians(p_rot->getValue().x), glm::vec3(1.0f, 0.0f, 0.0f));
	temp = glm::rotate(temp, glm::radians(p_rot->getValue().y), glm::vec3(0.0f, 1.0f, 0.0f));
	temp = glm::rotate(temp, glm::radians(p_rot->getValue().z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	temp = glm::scale(temp , p_scale->getValue());
	
	transforms = temp;
	
	position = p_pos->getValue();
	rotation = p_rot->getValue();
	scale = p_scale->getValue();
}

glm::vec3 Entity3D::getWorldPosition()
{
	glm::vec4 temp = glm::vec4(position.x, position.y, position.z,1.0f);
	glm::mat4 temp_matrix = glm::mat4(1.0f);
	applyParentsMatrices(temp_matrix);
	temp_matrix = transforms * temp_matrix;
	temp = temp * temp_matrix;
	//~ 
	return glm::vec3(temp.x, temp.y, temp.z);
	
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

void Entity3D::applyParentsMatrices(glm::mat4 & source_matrix)
{
	
	std::vector<Entity3D*> parents  = getParents();
	
	for (int i = 0; i < parents.size(); i++)
	{
		source_matrix = parents[i]->transforms * source_matrix;
	}	
	
	
}

Entity3D::~Entity3D()
{
	
}

