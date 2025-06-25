#include "EditorCameraSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/CameraComponent.h"
#include "Application.h"

#include <iostream>

namespace Aozora {

	EditorCameraSystem::EditorCameraSystem()
	{

	}

	void EditorCameraSystem::update()
	{
		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();
		auto view = registry.view<CameraComponent, TransformComponent>();

		for (const auto entity : view) { // change this we are currently rendering multiple cameras

			auto& camera = view.get<CameraComponent>(entity);

			if (camera.isActive()) {
				auto& transform = view.get<TransformComponent>(entity);

				glm::vec3 currentPos = camera.m_pos + transform.pos;
				
				

				glm::quat rotQuart = glm::quat(glm::radians(transform.rot));

	
				camera.m_forward = rotQuart * glm::vec3(0.0f, 0.0f, -1.0f);
				camera.m_forward = glm::normalize(camera.m_forward); 

				camera.m_right = rotQuart * glm::vec3(1.0f, 0.0f, 0.0f);
				camera.m_right = glm::normalize(camera.m_right);

				camera.m_up = glm::normalize(glm::cross(camera.m_right, camera.m_forward));
				
				camera.m_proj = glm::perspective(glm::radians(camera.m_fovY), (float)camera.m_viewPortX / camera.m_viewPortY, 0.1f, 1000.0f);
				camera.m_view = glm::lookAt(currentPos, currentPos + camera.m_forward, camera.m_up);
				
			}

		}
		
	}
}