#include "gizmo.h"

#include "../utils/mesh_utils.h"

Gizmo::Gizmo()
{
	position = glm::vec3(0.0f,0.0f,0.0f);
	rotation = glm::vec3(0.0f,0.0f,0.0f);
	scale = glm::vec3(1.0f,1.0f,1.0f);	
	
	
	TranslateHandle * trans_x = new TranslateHandle();
	trans_x->axis = 0;
	handles.push_back(trans_x);
	
	TranslateHandle * trans_y = new TranslateHandle();
	trans_y->axis = 1;
	handles.push_back(trans_y);
	
	TranslateHandle * trans_z = new TranslateHandle();
	trans_z->axis = 2;
	handles.push_back(trans_z);
}

// copy constructor
Gizmo::Gizmo(const Gizmo& other)
{
	
	//~ printf("Gizmo COPY CONSTRUCTOR\n");

	
	transforms = other.transforms;	
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;

	
	setName("copy_name");
}

void Gizmo::buildVbo()
{
	
	//~ printf("--- START VBO initialization \n");
	
	// build handles vbos
	for (int i = 0; i < handles.size(); i++)
	{


		TranslateHandle * p_trans = nullptr;
		
		if( p_trans = dynamic_cast<TranslateHandle*>(handles[i])){
			p_trans->buildVbo();
		}
		//~ handles[i].buildVbo();

	}


	//~ printf("--- END VBO initialization \n");
}


void Gizmo::draw(Shader & _shader, Camera & camera){
	
	//~ printf("drawing Gizmo !!!!!\n");
	//~ 
	//~ printf("\tnum handles : %d\n", handles.size());
	//~ printf("----------------------------\n");
	
	//~ _shader.useProgram();
	
	glm::mat4 model = glm::mat4(1.0f);
	
	model = target_object->transforms * model;
	
	target_object->applyParentsMatrices(model);
	
	glm::vec3 world_pos = glm::vec3( model[3][0], model[3][1], model[3][2]);
	
	float cam_distance = glm::distance(world_pos, camera.position);
	//~ printf("cam distance -> %.3f \n", cam_distance);
	float dist_square = cam_distance * cam_distance;
	float scale = cam_distance * 0.2;
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	// rotate to lay down X Axis
	

	
	

	
	TranslateHandle * p_translate = nullptr;
	
	for (int i = 0; i < handles.size(); i++)
	{
		if( p_translate = dynamic_cast<TranslateHandle*>(handles[i]))
		{
			
			p_translate->draw(_shader, camera, model);
		}
	}
	


		
}
void Gizmo::setName(std::string str_name)
{

	for (int i = 0; i < str_name.size(); i++)
	{
		name[i] = str_name[i];
	}
	
}



glm::vec3 Gizmo::getWorldPosition()
{
	glm::vec4 temp = glm::vec4(position.x, position.y, position.z,1.0f);
	glm::mat4 temp_matrix = glm::mat4(1.0f);
	
	temp_matrix = transforms * temp_matrix;
	temp = temp * temp_matrix;
	//~ 
	return glm::vec3(temp.x, temp.y, temp.z);
	
}


Gizmo::~Gizmo()
{
	
}

