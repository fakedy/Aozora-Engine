#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

class EditorCamera : public Camera{


public:


	glm::mat4 getProjection();
	glm::mat4 getView();
	void calcProjection();
	void update();

private:
	float movspeed = 10.0f;

};