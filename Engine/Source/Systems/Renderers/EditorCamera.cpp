#include "EditorCamera.h"
#include "Systems/Input.h"
#include "Systems/Time.h"


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

void EditorCamera::update()
{

	if (Input::getKeyDown(Input::Key::W)) {
		m_pos = m_pos + glm::vec3(0.0f, 0.0f, 1.0f) * movspeed * Time::deltaTime;
	}
	else
		if (Input::getKeyDown(Input::Key::S)) {
			m_pos = m_pos + glm::vec3(0.0f, 0.0f, -1.0f) * movspeed * Time::deltaTime;
		}

	if (Input::getKeyDown(Input::Key::D)) {
		m_pos = m_pos + glm::vec3(-1.0f, 0.0f, 0.0f) * movspeed * Time::deltaTime;
	}
	else
		if (Input::getKeyDown(Input::Key::A)) {
			m_pos = m_pos + glm::vec3(1.0f, 0.0f, 0.0f) * movspeed * Time::deltaTime;
		}

	if (Input::getKeyDown(Input::Key::SPACE)) {
		m_pos = m_pos + glm::vec3(0.0f, -1.0f, 0.0f) * movspeed * Time::deltaTime;
	}
	else
		if (Input::getKeyDown(Input::Key::LEFT_CONTROL)) {
			m_pos = m_pos + glm::vec3(0.0f, 1.0f, 0.0f) * movspeed * Time::deltaTime;
		}

	if (Input::getKeyPressed(Input::Key::E)) {
		movspeed = movspeed + 10.0f;
	}
	else if (Input::getKeyPressed(Input::Key::Q)) {
		movspeed = movspeed - 10.0f;
	}
}
