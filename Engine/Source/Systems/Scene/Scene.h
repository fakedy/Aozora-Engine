#pragma once
#include <entt/entt.hpp>
#include <memory>
#include "Systems/Renderers/EditorCamera.h"
#include "Systems/Renderers/Renderer.h"
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
	
		Scene(std::shared_ptr<Renderer> renderer, std::shared_ptr<entt::registry> registry);

		// ECS SYSTEM
		std::shared_ptr<entt::registry> m_registry;
		std::shared_ptr<Renderer> m_renderer;

		const char* m_sceneName{ "New Scene" };
		std::shared_ptr<CameraComponent> m_activeCamera;

		void update();

		void renderScene();
		void renderEditorScene(std::shared_ptr<EditorCamera> editorCamera);
		void editorUpdate(std::shared_ptr<EditorCamera> editorCamera);

		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
		OpenglShader m_gridShader{ OpenglShader("Resources/Shaders/v_grid.glsl", "Resources/Shaders/f_grid.glsl") };
	private:

		
	};
}
