#include "EditorLayer.h"
#include "Application.h"
#include "Systems/Input.h"
#include "glm/glm.hpp"
#include <Systems/Time.h>

void EditorLayer::onUpdate()
{

	// update systems here instead of application

	auto& app = Aozora::Application::getApplication();


	// TEMPORARY
	if (Input::getKeyDown(Input::Key::MOUSE_BUTTON_RIGHT)) {

		if (Input::getKeyDown(Input::Key::W)) {
			transformComponent->pos += cameraComponent->m_forward * movspeed * Time::deltaTime;
		}
		else
			if (Input::getKeyDown(Input::Key::S)) {
				transformComponent->pos -= cameraComponent->m_forward * movspeed * Time::deltaTime;
			}

		if (Input::getKeyDown(Input::Key::D)) {
			transformComponent->pos += cameraComponent->m_right * movspeed * Time::deltaTime;
		}
		else
			if (Input::getKeyDown(Input::Key::A)) {
				transformComponent->pos -= cameraComponent->m_right * movspeed * Time::deltaTime;
			}

		if (Input::getKeyDown(Input::Key::SPACE)) {
			transformComponent->pos += cameraComponent->m_up * movspeed * Time::deltaTime;
		}
		else
			if (Input::getKeyDown(Input::Key::LEFT_CONTROL)) {
				transformComponent->pos -= cameraComponent->m_up * movspeed * Time::deltaTime;
			}

		if (Input::getKeyPressed(Input::Key::E)) {
			movspeed = movspeed + 10.0f;
		}
		else if (Input::getKeyPressed(Input::Key::Q)) {
			movspeed = movspeed - 10.0f;
		}

		Input::MouseData data = Input::getMousePos();
		float xOffset = data.x - lastX;
		float yOffset = lastY - data.y;
		lastX = data.x;
		lastY = data.y;

		if (!mouseHeld) { // make sure we dont get sudden camera movement

			xOffset = 0;
			yOffset = 0;

		}

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.9f) {
			pitch = 89.9f;
		}
		if (pitch < -89.9f) {
			pitch = -89.9f;
		}

		mouseHeld = true;
		transformComponent->rot.x = pitch;
		transformComponent->rot.y = -yaw;
		transformComponent->rot.z = 0.0f;

			/*
		cameraComponent->m_forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraComponent->m_forward.y = sin(glm::radians(pitch));
		cameraComponent->m_forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		*/

	}
	else {

		mouseHeld = false;
	}





	// no difference yet
	if (state == EditorState::EDIT) {

		app.m_cameraSystem->update();
		m_editorCameraSystem->update();

		app.getCurrentScene().update();

		app.getRenderer().render();
	}
	else {

		app.m_cameraSystem->update();
		m_editorCameraSystem->update();

		app.getCurrentScene().update();

		app.getRenderer().render();

	}



}
