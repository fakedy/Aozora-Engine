#pragma once
#include <entt/entt.hpp>
#include <memory>
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/CameraComponent.h"
#include "Systems/ECS/Components/MeshComponent.h"
#include "Systems/Renderers/Opengl/OpenglShader.h"
#include "..\ResourceManager\ResourceManager.h"

namespace Aozora {

	class Scene {
	public:

		int m_gameViewPortX{ 1920 };
		int m_gameViewPortY{ 1080 };
	
		Scene();

		// ECS SYSTEM
		std::shared_ptr<entt::registry> m_registry;

		const char* m_sceneName{ "New Scene" };
		std::shared_ptr<CameraComponent> m_activeCamera;

		void update();

		entt::registry& getRegistry();


	private:

		
	};
}
