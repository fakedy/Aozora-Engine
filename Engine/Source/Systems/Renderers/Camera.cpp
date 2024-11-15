#include "Camera.h"

glm::mat4 Camera::getProjection()
{
	proj = glm::perspective(glm::radians(45.0f), (float)1920 / 1080, 0.1f, 100.0f);
	return proj;
}
