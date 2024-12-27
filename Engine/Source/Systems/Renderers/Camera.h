#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


class Camera {
public:


	glm::mat4 getProjection();
	glm::mat4 getView();
	int m_viewPortX = 1920;
	int m_viewPortY = 1080;
	float m_fovY = 45;

private:
protected:
	glm::mat4 m_proj = glm::mat4(1.0f);
	glm::mat4 m_view = glm::mat4(1.0f);
	glm::vec3 m_pos = glm::vec3(0.0f);

};