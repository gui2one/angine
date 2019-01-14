#include "camera.h"


Camera::Camera()
{
	position = glm::vec3(0.0f, -5.0f, 2.0f);
	target_position = glm::vec3(0.0f, 0.0f, 0.0f);
	up_vector = glm::vec3(0.0f, 0.0f, 1.0f);
	
}

Camera::Camera(float ratio, float _fov_angle)
{
	position = glm::vec3(0.0f, -5.0f, 2.0f);
	target_position = glm::vec3(0.0f, 0.0f, 0.0f);
	up_vector = glm::vec3(0.0f, 0.0f, 1.0f);
	projection = glm::mat4(1.0f) * glm::perspective(_fov_angle, ratio, 0.01f, 100.0f);
}

void Camera::setProjection(glm::mat4 _proj)
{
	projection = _proj;
}


