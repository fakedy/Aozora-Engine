#pragma once
#include <entt/entt.hpp>
#include <memory>
#include "Systems/Renderers/Camera.h"
#include "Systems/Renderers/EditorCamera.h"
#include "Systems/Renderers/Renderer.h"
#include "Systems/ECS/Components/ModelComponent.h"
#include "Systems/ECS/Components/TransformComponent.h"

namespace Aozora {

	class Scene {
	public:
	
		Scene(std::shared_ptr<Renderer> renderer);

		// ECS SYSTEM
		std::shared_ptr<entt::registry> m_registry;
		std::shared_ptr<Renderer> m_renderer;

		const char* m_sceneName{ "New Scene" };


		void update();

		void renderScene(std::shared_ptr<EditorCamera> camera);
		void editorUpdate(std::shared_ptr<EditorCamera> camera);

	private:


	};
}
