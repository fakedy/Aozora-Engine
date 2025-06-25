#pragma once
#include <memory>
#include "AozoraAPI/Aozora.h"
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

		// getter for application singleton
		static Application& getApplication() {
			return *m_appInstance;
		}

		Scene& getCurrentScene() {
			return *m_currentScene.get();
		}

		ResourceManager& getResourceManager() {
			return *m_resourceManager.get();
		}

		Renderer& getRenderer() {
			return *m_renderer.get();
		}

		void createNewScene();

		inline Window& getWindow() { return *m_window; }

		std::unique_ptr<CameraSystem> m_cameraSystem;
		

	private:
		static Application* m_appInstance;

		Window* m_window;
		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<ResourceManager> m_resourceManager;
		std::unique_ptr<Scene> m_currentScene;
		
		Window::WindowProps props;
		

	};

}

