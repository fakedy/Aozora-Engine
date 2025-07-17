#pragma once
#include <memory>
#include "AozoraAPI/Aozora.h"
#include "Systems/Windows/Window.h"
#include "Systems/Renderers/IrenderAPI.h"
#include "Systems/Layers/LayerStack.h"
#include "Systems/Layers/ImguiLayer.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/Scene/Scene.h"
#include "Systems/CameraSystem.h"
#include "Systems/Renderers/SceneRenderer.h"

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

		IrenderAPI& getRenderAPI() {
			return *m_renderAPI.get();
		}

		SceneRenderer& getRenderer() {
			return *m_sceneRenderer.get();
		}

		void createNewScene();

		inline Window& getWindow() { return *m_window; }

		std::unique_ptr<CameraSystem> m_cameraSystem;
		

	private:
		static Application* m_appInstance;

		Window* m_window;
		std::unique_ptr<IrenderAPI> m_renderAPI;
		std::unique_ptr<SceneRenderer> m_sceneRenderer;
		std::unique_ptr<ResourceManager> m_resourceManager;
		std::unique_ptr<Scene> m_currentScene; // hmmm
		
		Window::WindowProps props;
		

	};

}

