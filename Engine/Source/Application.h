#pragma once
#include <memory>
#include "Engine.h"
#include "Systems/Windows/Window.h"
#include "Systems/Renderers/Renderer.h"
#include "Systems/Layers/LayerStack.h"
#include "Systems/Layers/ImguiLayer.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/Scene/Scene.h"
#include "Systems/CameraSystem.h"

namespace Aozora {

	class Application
	{

	public:

		Application(const char* title);
		LayerStack* layerStack;

		void run();

		// singleton for app handle
		static Application& getApplication() {
			return *m_appInstance;
		}

		inline Window& getWindow() { return *m_window; }

		std::shared_ptr<Scene> m_currentScene;
		

	private:
		static Application* m_appInstance;

		Window* m_window;
		std::shared_ptr<Renderer> m_renderer;
		ResourceManager& m_resourceManager;
		
		std::unique_ptr<CameraSystem> m_cameraSystem;
		std::shared_ptr<entt::registry> m_registry;
		

	};

}

