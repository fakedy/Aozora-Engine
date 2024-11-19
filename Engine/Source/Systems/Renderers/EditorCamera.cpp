#include "EditorCamera.h"

glm::mat4 EditorCamera::getProjection()
{
	return m_proj;
}

glm::mat4 EditorCamera::getView()
{
	return glm::translate(m_view, m_pos);
}

void EditorCamera::calcProjection()
{
	m_proj = glm::perspective(glm::radians(m_fovY), (float)m_viewPortX / m_viewPortY, 0.1f, 1000.0f);
}
