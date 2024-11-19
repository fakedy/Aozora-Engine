#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EditorCamera {


public:

	int m_viewPortX = 1920;
	int m_viewPortY = 1080;
	glm::mat4 getProjection();
	glm::mat4 getView();
	void calcProjection();

private:

	float m_fovY = 45;
	glm::mat4 m_proj = glm::mat4(1.0f);
	glm::mat4 m_view = glm::mat4(1.0f);

	glm::vec3 m_pos = glm::vec3(0.0f);

};