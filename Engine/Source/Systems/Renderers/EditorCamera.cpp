#include "EditorCamera.h"
#include "Systems/Input.h"
#include "Systems/Time.h"
#include <iostream>


glm::mat4 EditorCamera::getProjection()
{
	return m_proj;
}

glm::mat4 EditorCamera::getView()
{

	m_right = glm::normalize(glm::cross(m_forward, m_up));

	m_view = glm::lookAt(m_pos, m_pos + m_forward, m_up);
	glm::translate(m_view, m_pos);

	return m_view;
}

void EditorCamera::calcProjection()
{
	m_proj = glm::perspective(glm::radians(m_fovY), (float)m_viewPortX / m_viewPortY, 0.1f, 1000.0f);
}

void EditorCamera::update()
{


	if (Input::getKeyDown(Input::Key::MOUSE_BUTTON_RIGHT)) {

		if (Input::getKeyDown(Input::Key::W)) {
			m_pos += m_forward * movspeed * Time::deltaTime;
		}
		else
			if (Input::getKeyDown(Input::Key::S)) {
				m_pos -= m_forward * movspeed * Time::deltaTime;
			}

		if (Input::getKeyDown(Input::Key::D)) {
			m_pos += m_right * movspeed * Time::deltaTime;
		}
		else
			if (Input::getKeyDown(Input::Key::A)) {
				m_pos -= m_right * movspeed * Time::deltaTime;
			}

		if (Input::getKeyDown(Input::Key::SPACE)) {
			m_pos += m_up * movspeed * Time::deltaTime;
		}
		else
			if (Input::getKeyDown(Input::Key::LEFT_CONTROL)) {
				m_pos -= m_up * movspeed * Time::deltaTime;
			}

		if (Input::getKeyPressed(Input::Key::E)) {
			movspeed = movspeed + 10.0f;
		}
		else if (Input::getKeyPressed(Input::Key::Q)) {
			movspeed = movspeed - 10.0f;
		}


		Input::MouseData data = Input::getMousePos();

		const float sensitivity = 0.1f;


		float xOffset = data.x - lastX;
		float yOffset = lastY - data.y;
		lastX = data.x;
		lastY = data.y;

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;


		m_forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		m_forward.y = sin(glm::radians(pitch));
		m_forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));


		m_forward = glm::normalize(m_forward);
	}




}
