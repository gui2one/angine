#include "raycaster.h"


Raycaster::Raycaster()
{
	printf("I am a raycaster ....\n");
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

static std::vector<int> check_triangles(Ray& ray, Object* object)
{
	printf("object name --> %s\n", object->name);
	
	std::vector<int> triangles_ids;
	
	//~ object->applyTransforms();
	
	//~ printf("Object transforms : translates \n");
	//~ printf("\t%.3f, %.3f, %.3f\n", object->transforms[3][0],object->transforms[3][1],object->transforms[3][2]);
	for (int i = 0; i < object->mesh.indices.size(); i += 3)
	{
		//~ printf("triangle number %d\n", i / 3);
		
		// ray vs plane intersection first
		
		
		
		glm::vec3 vtx_a = object->mesh.vertices[object->mesh.indices[i]].position;
		vec_mult_by_matrix(vtx_a, object->transforms);
		//~ printf("vertex position -->  %.2f, %.2f, %.2f\n", vtx_a.x, vtx_a.y, vtx_a.z);
		//~ printf("Object position -->  %.2f, %.2f, %.2f\n", object->position.x, object->position.y, object->position.z);
		
		glm::vec3 vtx_b = object->mesh.vertices[object->mesh.indices[i+1]].position;
		vec_mult_by_matrix(vtx_b, object->transforms);
		
		glm::vec3 vtx_c = object->mesh.vertices[object->mesh.indices[i+2]].position;
		vec_mult_by_matrix(vtx_c, object->transforms);
		
		glm::vec3 plane_pos = (vtx_a + vtx_b + vtx_c);
		plane_pos = plane_pos / 3.0f;
		//~ printf("--------------------------------\n");
		//~ printf("plane pos -> %.3f, %.3f, %.3f\n", plane_pos.x, plane_pos.y, plane_pos.z );
		
		
		glm::vec3 plane_normal = glm::normalize(glm::cross( (vtx_b - vtx_a) , (vtx_c - vtx_a) ));
		//~ printf("plane normal -> %.3f, %.3f, %.3f\n", plane_normal.x, plane_normal.y, plane_normal.z );
		
		glm::vec3 hitP = glm::vec3(0.0f);
		int hit = ray_plane_intersect(plane_normal, plane_pos, ray.pos, ray.dir, hitP);
		
		if(hit)
		{
			if( point_in_triangle( cartesian_to_barycentric(hitP, vtx_a, vtx_b, vtx_c))){
				//~ printf("just hit a fucking triangle !!!!! ID : %d\n", i/3);
				//~ printf("\thitP -> %.3f, %.3f, %.3f\n", hitP.x, hitP.y, hitP.z); 
				
				triangles_ids.push_back(i/3);
			}
		}
	}
	
	return triangles_ids;
	
}



bool Raycaster::fromCamera(
						Window* _window, 
						Camera& _camera, 
						std::vector<Entity3D*> _target_objects, 
						std::vector<Entity3D*>& _result_objects)
{
	printf("camera position : %.3f, %.3f, %.3f\n", _camera.position.x, _camera.position.y, _camera.position.z);
	
	double pos_x, pos_y;
	glfwGetCursorPos(_window->win, &pos_x, &pos_y);
	
	float width = _window->width;
	float height = _window->height;

	
	printf("cursor window pos : %.3f, %.3f \n", pos_x, pos_y);

		
		for (int i = 0; i < _target_objects.size(); i++)
		{
			Entity3D * cur = _target_objects[i];
			
			Object * p_object = nullptr;
			if( p_object = dynamic_cast<Object *>(cur)){
				
				BoundingBox AABB = p_object->computeAABB();
				
				glm::vec3 bbox_pos = AABB.position;
				glm::vec3 bbox_size = AABB.size;
				glm::vec3 bbox_center = bbox_pos + glm::vec3(bbox_size.x/2.0f, bbox_size.y/2.0f, bbox_size.y/2.0f) ;	

				
				
				float x = (2.0f * pos_x) / width - 1.0f;
				float y = 1.0f - (2.0f * pos_y) / height;
				
				
				glm::vec3 planeN = glm::normalize(p_object->position - _camera.position);
				glm::vec3 planeP = bbox_center;
				glm::vec3 pointP = glm::vec3(x, y , 1.0f);

				glm::vec3 hitP = glm::vec3(0.0f);
				
				
				Ray  ray = ray_from_camera(_window, x, y);
				
				int hit = ray_plane_intersect(planeN, planeP, ray.pos , ray.dir, hitP);		
				
				if(hit)
				{
					if( hitP.x > bbox_pos.x && hitP.x < bbox_pos.x + bbox_size.x)
					{
						
						if( hitP.y > bbox_pos.y && hitP.y < bbox_pos.y + bbox_size.y)
						{
							
							if( hitP.z > bbox_pos.z && hitP.z < bbox_pos.z + bbox_size.z)
							{	
								std::vector<int> triangles_ids = check_triangles(ray, p_object);
								
								if( triangles_ids.size() > 0){
													
									printf("hit object ID -> %d from Raycaster ----------------- !!!! yes \n", i);
									_result_objects.push_back(p_object);
								}
								
								//~ cur_object_selected = i;
								//~ return _target_objects[i];
							}
						}
					}
					
				}
			}
		}

	return false;
}


