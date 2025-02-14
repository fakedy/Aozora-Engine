#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>


class Camera {
public:


	glm::mat4 getProjection();
	glm::mat4 getView();
	glm::vec3 getPos();
	int m_viewPortX{1920};
	int m_viewPortY{ 1080 };
	float m_fovY{ 45.0f };

private:
protected:
	glm::mat4 m_proj = glm::mat4(1.0f);
	glm::mat4 m_view = glm::mat4(1.0f);
	glm::vec3 m_pos = glm::vec3(0.0f);

	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);

	float lastX = 1920 / 2.0f;
	float lastY = 1080 / 2.0f;

	float yaw = -90.0f; // rotate
	float pitch = 0.0f; // up and down

};