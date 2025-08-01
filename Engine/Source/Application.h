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
#include <functional>
#include <queue>
#include <Systems/SceneManager/SceneManager.h>
#include <Systems/ScriptManager/ScriptSystem.h>

namespace Aozora {

	class Project;

	class Application
	{

	public:

		Application(const char* title);
		LayerStack* layerStack;

		void run();

		// TODO stop with this 
		// getter for application singleton
		static Application& getApplication() {
			return *m_appInstance;
		}

		Scene& getCurrentScene();

		ResourceManager& getResourceManager() {
			return *m_resourceManager.get();
		}

		SceneManager& getSceneManager() {
			return *m_sceneManager.get();
		}

		IrenderAPI& getRenderAPI() {
			return *m_renderAPI.get();
		}

		SceneRenderer& getRenderer() {
			return *m_sceneRenderer.get();
		}
		ScriptSystem& getScriptSystem() {
			return *m_scriptSystem.get();
		}


		inline Window& getWindow() { return *m_window; }

		std::unique_ptr<CameraSystem> m_cameraSystem;

		void queueAction(std::function<void()> func);
		void processActions();


		std::unique_ptr<Project> m_project;

	private:
		static Application* m_appInstance;

		Window* m_window;
		std::unique_ptr<IrenderAPI> m_renderAPI;
		std::unique_ptr<ResourceManager> m_resourceManager;
		std::unique_ptr<SceneRenderer> m_sceneRenderer;
		std::unique_ptr<SceneManager> m_sceneManager;
		std::unique_ptr<ScriptSystem> m_scriptSystem;
		
		Window::WindowProps props;

		std::queue<std::function<void()>> m_actionQueue;
		

	};

}

