#include "CameraSystem.h"



namespace Aozora {

	CameraSystem::CameraSystem(std::shared_ptr<entt::registry> registry) : m_registry(registry)
	{


	}

	void CameraSystem::update()
	{

		
		auto view = m_registry->view<CameraComponent, TransformComponent>();

		for (const auto entity : view) { // change this we are currently rendering multiple cameras

			auto& transform = view.get<TransformComponent>(entity);
			auto& camera = view.get<CameraComponent>(entity);


			camera.setProjection(glm::perspective(glm::radians(camera.getFov()), (float)camera.m_viewPortX / camera.m_viewPortY, 0.1f, 1000.0f));


			camera.setView(glm::translate(camera.getView(), camera.getPos()));





		}
		

	}
}

