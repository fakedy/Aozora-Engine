#include "Camera.h"

glm::mat4 Camera::getProjection()
{
	m_proj = glm::perspective(glm::radians(m_fovY), (float)m_viewPortX / m_viewPortY, 0.1f, 1000.0f);
	return m_proj;
}

glm::mat4 Camera::getView()
{

	return glm::translate(m_view, m_pos);
}

glm::vec3 Camera::getPos()
{
	return m_pos;
}