#pragma once
#include <entt/entt.hpp>
#include <memory>
#include "Systems/Renderers/Camera.h"
#include "Systems/Renderers/EditorCamera.h"
#include "Systems/Renderers/Renderer.h"
#include "Systems/ECS/Components/ModelComponent.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/CameraComponent.h"
#include "Systems/Renderers/Opengl/OpenglShader.h"

namespace Aozora {

	class Scene {
	public:
	
		Scene(std::shared_ptr<Renderer> renderer);

		// ECS SYSTEM
		std::shared_ptr<entt::registry> m_registry;
		std::shared_ptr<Renderer> m_renderer;

		const char* m_sceneName{ "New Scene" };
		std::shared_ptr<Camera> m_activeCamera;

		void update();

		void renderScene();
		void renderEditorScene(std::shared_ptr<EditorCamera> editorCamera);
		void editorUpdate(std::shared_ptr<EditorCamera> editorCamera);

		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
	private:


	};
}
