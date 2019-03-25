#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "pch.h"
#include "camera.h"
#include "window.h"
#include "entity3d.h"
#include "object.h"

struct Ray{
	
	glm::vec3 pos;
	glm::vec3 dir;
	
};

class Raycaster
{
	public:
		Raycaster();
		
		bool intersectObjects(Window* _window, Camera& _camera, std::vector<Entity3D*> _target_objects, std::vector<Entity3D*>& _result_objects);
		bool intersectGizmos(Window* _window, Camera& _camera, std::vector<Gizmo*> _target_gizmos, std::vector<Gizmo*>& _result_gizmos);
			
	private:
		/* add your private declarations */
};

#endif /* RAYCASTER_H */ 
