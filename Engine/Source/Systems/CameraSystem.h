#pragma once
#include <memory>
#include <entt/entt.hpp>
#include <memory>
#include "Systems/Renderers/Camera.h"
#include "Systems/Renderers/EditorCamera.h"
#include "Systems/Renderers/Renderer.h"
#include "Systems/ECS/Components/ModelComponent.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/CameraComponent.h"

namespace Aozora {

	class CameraSystem
	{
	public:

		CameraSystem(std::shared_ptr<entt::registry> registry);

		void update();



	private:


		std::shared_ptr<entt::registry> m_registry;

	};
}

