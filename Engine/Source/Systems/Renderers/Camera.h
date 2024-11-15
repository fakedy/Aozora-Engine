#pragma once
#include <glm/gtc/matrix_transform.hpp>


class Camera {
public:


	glm::mat4 getProjection();


private:
	glm::mat4 proj = glm::mat4(1.0f);

};