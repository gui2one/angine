#ifndef RAYCASTER_H
#define RAYCASTER_H

#include "pch.h"
#include "camera.h"
#include "window.h"
#include "entity3d.h"
#include "object.h"

struct Ray
{
	
	glm::vec3 pos;
	glm::vec3 dir;
	
};

struct DragHandleData
{
	Entity3D * target_object;
	glm::vec3 plane_position;
	glm::vec3 plane_normal;
	glm::vec3 start_local_position;
};

class Raycaster
{
	public:
		Raycaster();
		
		bool intersectBoudingBox(Window* _window, Camera& _camera, Entity3D* _target_object);
		bool intersectObjects(Window* _window, Camera& _camera, std::vector<Entity3D*> _target_objects, std::vector<Entity3D*>& _result_objects);
		Gizmo* intersectGizmos(Window* _window, Camera& _camera, std::vector<Gizmo*> _target_gizmos, DragHandleData& _drag_data);
		
		void dragHandle(DragHandleData& _data);
	private:
		/* add your private declarations */
};

#endif /* RAYCASTER_H */ 
