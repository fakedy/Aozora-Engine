#include "CameraSystem.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/CameraComponent.h"

#include <iostream>

namespace Aozora {

	CameraSystem::CameraSystem(std::shared_ptr<entt::registry> registry) : m_registry(registry)
	{

	}

	void CameraSystem::update()
	{

		auto view = m_registry->view<CameraComponent, TransformComponent>();

		for (const auto entity : view) { // change this we are currently rendering multiple cameras

			auto& camera = view.get<CameraComponent>(entity);

			if (camera.isActive()) {
				auto& transform = view.get<TransformComponent>(entity);

				camera.setPos(transform.pos);
				camera.setProjection(glm::perspective(glm::radians(camera.getFov()), (float)camera.m_viewPortX / camera.m_viewPortY, 0.1f, 1000.0f));
				camera.setView(glm::lookAt(camera.getPos(), camera.getPos() + camera.getForwardVector(), camera.getUpVector()));
				
			}

		}
		
	}
}