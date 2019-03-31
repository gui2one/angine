#include "raycaster.h"


Raycaster::Raycaster()
{
	//~ printf("I am a raycaster ....\n");
}

static void vec_mult_by_matrix( glm::vec3 & _vec, glm::mat4 & _mat, bool invert = false){
	
	glm::vec4 temp_vec4 = glm::vec4(_vec.x, _vec.y, _vec.z,1.0f);
	
	
	if( invert){
		_vec = glm::inverse(_mat) * temp_vec4 ;
	} else{
		_vec = _mat * temp_vec4;
		//~ printf("__VEC X : %.3f\n", _vec.x);
	}

}

static Ray ray_from_camera(Window* window, float x, float y)
{
	double pos_x, pos_y;
	glfwGetCursorPos(window->win, &pos_x, &pos_y);
	
	float width = window->width;
	float height = window->height;	
	
	
	glm::vec3 screen_pos = glm::vec3(x, y , 1.0f);
	
				
				glm::vec3 rayDir = glm::vec3(0.0f, 0.0f , -1.0f);
				
				glm::mat4 projection = window->camera.projection;
				glm::mat4 view = glm::mat4(1.0f);

				
				
				// not sure why I need this, but it gets rid off a nasty offset 
				// found a solution here : https://stackoverflow.com/questions/48514387/can-anyone-explain-this-small-offset-from-ray-casting-by-mouse-click?rq=1
				// but the guy says he forced projection[3][3] to be 0.0, I have to do 1.0f for this to work
				
				projection[3][3] = 1.0f; 

				glm::vec3 up_vector = glm::vec3(0.0f,0.0f,1.0f);

				view *= glm::lookAt(
										window->camera.position, 
										window->camera.target_position, 
										glm::normalize(up_vector)
									);			
									
				glm::vec4 dir = inverse(projection * view)* glm::vec4(screen_pos.x, screen_pos.y, screen_pos.z, 1.0f) ;
				dir /= dir.w *0.5f;	
	
	
	Ray ray;
	ray.pos = window->camera.position;
	ray.dir = glm::vec3(dir.x, dir.y, dir.z);
	return ray;
}
		
static glm::vec3 cartesian_to_barycentric(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    
    
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    
    return glm::vec3(u,v,w);
}

static int ray_plane_intersect(glm::vec3 planeN, glm::vec3 planeP, glm::vec3 pointP, glm::vec3 rayDir, glm::vec3& hitP)
{
    glm::vec3 W = planeP - pointP;
    
    float K = glm::dot(W, planeN) / glm::dot( rayDir, planeN);
    
    hitP = pointP + ( K * rayDir);
    return  K>= 0.0 && K <= 1.0;
}

static bool point_in_triangle(glm::vec3 bary)
{
	bool result = 	0.0 <= bary.x && bary.x <= 1.0 &&
					0.0 <= bary.y && bary.y <= 1.0 &&
					0.0 <= bary.z && bary.z <= 1.0;
	return result;
}

static bool ray_triangle_intersect(Ray& ray, glm::vec3 vtx_a, glm::vec3 vtx_b, glm::vec3 vtx_c)
{
		glm::vec3 plane_pos = (vtx_a + vtx_b + vtx_c);
		plane_pos = plane_pos / 3.0f;

		
		
		glm::vec3 plane_normal = glm::normalize(glm::cross( (vtx_b - vtx_a) , (vtx_c - vtx_a) ));
		
		glm::vec3 hitP = glm::vec3(0.0f);
		int hit = ray_plane_intersect(plane_normal, plane_pos, ray.pos, ray.dir, hitP);	
		
		if( point_in_triangle( cartesian_to_barycentric(hitP, vtx_a, vtx_b, vtx_c)))
		{
			return true;
		}	
		
		return false;	
}
static std::vector<int> check_triangles(Ray& ray, Object* object)
{
	std::vector<int> triangles_ids;
	
	for (int i = 0; i < object->mesh.indices.size(); i += 3)
	{
		//~ printf("triangle number %d\n", i / 3);
		
		// ray vs plane intersection first
		
		glm::vec3 vtx_a = object->mesh.vertices[object->mesh.indices[i]].position;
		vec_mult_by_matrix(vtx_a, object->transforms);

		
		glm::vec3 vtx_b = object->mesh.vertices[object->mesh.indices[i+1]].position;
		vec_mult_by_matrix(vtx_b, object->transforms);
		
		glm::vec3 vtx_c = object->mesh.vertices[object->mesh.indices[i+2]].position;
		vec_mult_by_matrix(vtx_c, object->transforms);
				
		bool hit = ray_triangle_intersect(ray, vtx_a, vtx_b, vtx_c);
		
		if(hit)
		{				
			//~ printf("hit triangle !! \n");			
			triangles_ids.push_back(i/3);			
		}
	}
	
	return triangles_ids;
	
}

bool Raycaster::intersectBoudingBox(
						Window* _window, 
						Camera& _camera, 
						Entity3D* _target_object)
{
	
	
	double pos_x, pos_y;
	glfwGetCursorPos(_window->win, &pos_x, &pos_y);
	
	float width = _window->width;
	float height = _window->height;
	
	float x = (2.0f * pos_x) / width - 1.0f;
	float y = 1.0f - (2.0f * pos_y) / height;	
		

		
		
	Object * p_object = nullptr;
	
	if( p_object = dynamic_cast<Object *>(_target_object)){
		
		BoundingBox AABB = p_object->computeAABB();
		
		glm::vec3 bbox_pos = AABB.position;
		glm::vec3 bbox_size = AABB.size;
		glm::vec3 bbox_center = bbox_pos + glm::vec3(bbox_size.x/2.0f, bbox_size.y/2.0f, bbox_size.y/2.0f) ;		
		
	
		// build bbox out of triangles for intersection by ray
		
		std::vector<glm::vec3> bbox_vertices;
		
		bbox_vertices.push_back(bbox_pos);
		bbox_vertices.push_back(bbox_pos + glm::vec3( bbox_size.x,        0.0f,        0.0f));
		bbox_vertices.push_back(bbox_pos + glm::vec3(        0.0f,        0.0f, bbox_size.z));
		bbox_vertices.push_back(bbox_pos + glm::vec3( bbox_size.x,        0.0f, bbox_size.z));
		
		bbox_vertices.push_back(bbox_pos + glm::vec3(         0.0, bbox_size.y,        0.0f));
		bbox_vertices.push_back(bbox_pos + glm::vec3( bbox_size.x, bbox_size.y,        0.0f));
		bbox_vertices.push_back(bbox_pos + glm::vec3(        0.0f, bbox_size.y, bbox_size.z));
		bbox_vertices.push_back(bbox_pos + glm::vec3( bbox_size.x, bbox_size.y, bbox_size.z));
		
		
		
		std::vector<int> indices;
					
		indices.insert(indices.end(), {0, 2, 3});
		indices.insert(indices.end(), {0, 3, 1});
		
		indices.insert(indices.end(), {1, 3, 7});
		indices.insert(indices.end(), {1, 7, 5});
		
		indices.insert(indices.end(), {5, 7, 6});
		indices.insert(indices.end(), {5, 6, 4});
		
		indices.insert(indices.end(), {4, 6, 2});
		indices.insert(indices.end(), {4, 2, 0});
		
		indices.insert(indices.end(), {2, 6, 7});
		indices.insert(indices.end(), {2, 7, 3});
		
		indices.insert(indices.end(), {0, 4, 5});
		indices.insert(indices.end(), {0, 5, 1});
		
		
		// now perform triangle intersection
		
		Ray ray = ray_from_camera(_window, x, y);
		
		for (int id = 0; id < indices.size()/3; id++)
		{
			glm::vec3 vtx_a = bbox_vertices[indices[(id*3) + 0]];
			glm::vec3 vtx_b = bbox_vertices[indices[(id*3) + 1]];
			glm::vec3 vtx_c = bbox_vertices[indices[(id*3) + 2]];
			
			bool hit = ray_triangle_intersect(ray , vtx_a, vtx_b, vtx_c);
			
			if(hit)
			{
				//~ printf(" just hit boudingox triangle !!! for fuck sake !!!\n");
				return true;
			}
		}
		
	
	}
		
		
	
	
	return false;
}

bool Raycaster::intersectObjects(
						Window* _window, 
						Camera& _camera, 
						std::vector<Entity3D*> _target_objects, 
						std::vector<Entity3D*>& _result_objects)
{
	//~ printf("camera position : %.3f, %.3f, %.3f\n", _camera.position.x, _camera.position.y, _camera.position.z);
	
	double pos_x, pos_y;
	glfwGetCursorPos(_window->win, &pos_x, &pos_y);
	
	float width = _window->width;
	float height = _window->height;

	float x = (2.0f * pos_x) / width - 1.0f;
	float y = 1.0f - (2.0f * pos_y) / height;
	
	Ray  ray = ray_from_camera(_window, x, y);	
	//~ printf("cursor window pos : %.3f, %.3f \n", pos_x, pos_y);

		
	for (int i = 0; i < _target_objects.size(); i++)
	{
		Entity3D * cur = _target_objects[i];
		
		Object * p_object = nullptr;
		if( p_object = dynamic_cast<Object *>(cur)){
			
			bool hit_bbox = intersectBoudingBox(_window, _camera, p_object);
			
			if( hit_bbox)
			{
				std::vector<int> triangles_ids = check_triangles(ray, p_object);
				if( triangles_ids.size() > 0){
									
					//~ printf("hit object ID -> %d from Raycaster ----------------- !!!! yes \n", i);
					
					_result_objects.push_back(p_object);
				}				
			}
		}
	}
	
	if( _result_objects.size() > 0 ){
		
		sort( _result_objects.begin(), _result_objects.end(), [_camera](Entity3D* obj1, Entity3D* obj2 ){
			
				float dist1 = glm::distance(obj1->position, _camera.position);
				float dist2 = glm::distance(obj2->position, _camera.position);
				return (dist1 < dist2);
		} );
		return true;
	}
		
		

	return false;
}


Gizmo * Raycaster::intersectGizmos(
						Window* _window, 
						Camera& _camera, 
						std::vector<Gizmo*> _target_gizmos)
{
	double pos_x, pos_y;
	glfwGetCursorPos(_window->win, &pos_x, &pos_y);

	float width = _window->width;
	float height = _window->height;
	
	float x = (2.0f * pos_x) / width - 1.0f;
	float y = 1.0f - (2.0f * pos_y) / height;			
	
	Ray  ray = ray_from_camera(_window, x, y);	
		
	for (int i = 0; i < _target_gizmos.size(); i++)
	{
		
		glm::mat4 target_transforms = glm::mat4(1.0f);
		
		if( _target_gizmos[i]->target_object)
		{
			TranslationGizmo * p_translation = nullptr;
			
			if( p_translation = dynamic_cast<TranslationGizmo*>(_target_gizmos[i]))
			{	
				p_translation->target_object->applyTransforms();
				
				target_transforms = p_translation->target_object->transforms * target_transforms;			
				p_translation->target_object->applyParentsMatrices(target_transforms);
				
				glm::vec3 z_axis_2 = glm::vec3(0.0f, 0.0f, 1.0f);
				
				// do not multiply by the whole matrix, remove positon and scale
				
				glm::mat4 clean_mat = glm::mat4(1.0f);
				clean_mat[2][0] = target_transforms[2][0];
				clean_mat[2][1] = target_transforms[2][1];
				clean_mat[2][2] = target_transforms[2][2];
				vec_mult_by_matrix(z_axis_2, clean_mat, false);

				glm::vec3 world_pos = p_translation->target_object->getWorldPosition();
				
				//~ printf("world_pos value === %.3f, %.3f, %.3f\n", world_pos.x, world_pos.y, world_pos.z); 
				glm::vec3 planeN = glm::normalize(z_axis_2);
				
				glm::vec3 planeP = world_pos;
				glm::vec3 pointP = glm::vec3(x, y , 1.0f);
				glm::vec3 rayDir = glm::vec3(0.0f, 0.0f , -1.0f);			
									
				//~ glm::vec4 tempPointP = inverse(projection * view)* glm::vec4(pointP.x, pointP.y, pointP.z, 1.0f) ;
				//~ tempPointP /= tempPointP.w *0.5f;

				glm::vec3 hitP = glm::vec3(0.0f);
				int hit = ray_plane_intersect(planeN, planeP, ray.pos, ray.dir, hitP);	
				
				glm::vec4 local_pos = glm::vec4(hitP.x, hitP.y, hitP.z, 1.0f);
				
				glm::vec3 local_pos_vec3 = glm::inverse(target_transforms) * local_pos ;
				if(hit){
					//~ printf("------> gizmo hit <--------\n");
					
					float cam_dist = glm::distance( _camera.position, world_pos);
					//~ printf("\tlocal_pos_vec3 : %.3f, %.3f, %.3f\n", local_pos_vec3.x, local_pos_vec3.y, local_pos_vec3.z);
					if( fabs(local_pos_vec3.x) > fabs(local_pos_vec3.y)
						&&  fabs(local_pos_vec3.y) < 0.1 
						&&  local_pos_vec3.x < (p_translation->scale.x * cam_dist / 5.0f)
						&&  local_pos_vec3.x > 0.0)
					{
						//~ printf("local X : %.3f, local Y : %.3f\n", fabs(local_pos_vec3.x), fabs(local_pos_vec3.y));
						printf("nearest is X axis\n");
						
						//~ Handle * _result_handle = p_translation->handles[0];
						//~ printf("Handle pointer --> %x\n", _result_handle);
						p_translation->active_handle_id = 0;
						return p_translation;
					} else if(fabs(local_pos_vec3.x) < fabs(local_pos_vec3.y)
					&&  fabs(local_pos_vec3.x) < 0.1
					&&  local_pos_vec3.y < (p_translation->scale.y * cam_dist / 5.0f)
					&&  local_pos_vec3.y > 0.0)
					{
						
						//~ printf("local X : %.3f, local Y : %.3f\n", fabs(local_pos_vec3.x), fabs(local_pos_vec3.y));
						printf("nearest is Y axis\n");
						//~ Handle * _result_handle = p_translation->handles[1];
						//~ printf("Handle pointer --> %x\n", _result_handle);
						p_translation->active_handle_id = 1;
						return p_translation;
					}else{
						return nullptr;
					}	
				}				
			}
			
		}
	}
	
	
	return nullptr;
}
