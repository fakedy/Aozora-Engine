#include "EditorCameraSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Systems/ECS/Components/Components.h>
#include "Application.h"
#include "Systems/Input.h"
#include <Systems/Time.h>

#include <iostream>

namespace Aozora {

	EditorCameraSystem::EditorCameraSystem()
	{

	}

	void EditorCameraSystem::update(entt::registry& registry)
	{
		auto view = registry.view<CameraComponent, TransformComponent, EditorEntityTag>();

		for (const auto entity : view) {

			auto& camera = view.get<CameraComponent>(entity);

			auto& transform = view.get<TransformComponent>(entity);


			// TEMPORARY
			if (Input::getKeyDown(Input::Key::MOUSE_BUTTON_RIGHT)) {

				if (Input::getKeyDown(Input::Key::W)) {
					transform.pos += camera.m_forward * movspeed * Time::deltaTime;
				}
				else
					if (Input::getKeyDown(Input::Key::S)) {
						transform.pos -= camera.m_forward * movspeed * Time::deltaTime;
					}

				if (Input::getKeyDown(Input::Key::D)) {
					transform.pos += camera.m_right * movspeed * Time::deltaTime;
				}
				else
					if (Input::getKeyDown(Input::Key::A)) {
						transform.pos -= camera.m_right * movspeed * Time::deltaTime;
					}

				if (Input::getKeyDown(Input::Key::SPACE)) {
					transform.pos += camera.m_up * movspeed * Time::deltaTime;
				}
				else
					if (Input::getKeyDown(Input::Key::LEFT_CONTROL)) {
						transform.pos -= camera.m_up * movspeed * Time::deltaTime;
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
				transform.rot.x = pitch;
				transform.rot.y = -yaw;
				transform.rot.z = 0.0f;


			}
			else {

				mouseHeld = false;
			}


			// camera render code
			glm::vec3 currentPos = transform.pos;

			glm::quat rotQuart = glm::quat(glm::radians(transform.rot));

			camera.m_forward = rotQuart * glm::vec3(0.0f, 0.0f, -1.0f);
			camera.m_forward = glm::normalize(camera.m_forward);

			camera.m_right = rotQuart * glm::vec3(1.0f, 0.0f, 0.0f);
			camera.m_right = glm::normalize(camera.m_right);

			camera.m_up = glm::normalize(glm::cross(camera.m_right, camera.m_forward));

			camera.m_proj = glm::perspective(glm::radians(camera.m_fovY), (float)camera.m_viewPortWidth / camera.m_viewPortHeight, 0.1f, 10000.0f);
			camera.m_view = glm::lookAt(currentPos, currentPos + camera.m_forward, camera.m_up);


		}
		
	}
}