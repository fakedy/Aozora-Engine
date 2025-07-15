#include "EditorCameraSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Systems/ECS/Components/Components.h>
#include "Application.h"

#include <iostream>

namespace Aozora {

	EditorCameraSystem::EditorCameraSystem()
	{

	}

	void EditorCameraSystem::update()
	{
		// probably gonna use dependency injection instead
		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();
		auto view = registry.view<CameraComponent, TransformComponent, EditorEntityTag>();

		for (const auto entity : view) {

			auto& camera = view.get<CameraComponent>(entity);

			auto& transform = view.get<TransformComponent>(entity);

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