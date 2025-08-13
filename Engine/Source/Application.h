#pragma once
#include <memory>
#include "AozoraAPI/Aozora.h"
#include "Systems/Windows/Window.h"
#include "Systems/Renderers/IrenderAPI.h"
#include "Systems/Layers/LayerStack.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/AssetManager/AssetManager.h"
#include "Systems/Project/ProjectManager.h"
#include "Systems/Scene/Scene.h"
#include "Systems/CameraSystem.h"
#include "Systems/Renderers/SceneRenderer.h"
#include <functional>
#include <queue>
#include <Systems/SceneManager/SceneManager.h>
#include <Systems/ScriptManager/ScriptSystem.h>
#include <Context.h>
#include <Systems/Events/Events.h>
#include <Systems/Events/EventSystem.h>

namespace Aozora {

	class Project;

	class Application : public IEventListener
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

		ResourceManager& getResourceManager() {
			return *m_resourceManager.get();
		}


		Resources::AssetManager& getAssetManager() {
			return *m_assetManager.get();
		}

		SceneManager& getSceneManager() {
			return *m_sceneManager.get();
		}

		IrenderAPI& getRenderAPI() {
			return *m_renderAPI.get();
		}

		Graphics::SceneRenderer& getRenderer() {
			return *m_sceneRenderer.get();
		}
		ScriptSystem& getScriptSystem() {
			return *m_scriptSystem.get();
		}


		inline Window& getWindow() { return *m_window; }

		std::unique_ptr<CameraSystem> m_cameraSystem;

		void queueAction(std::function<void()> func);
		void processActions();
		
		void createProject();


		std::unique_ptr<IrenderAPI> m_renderAPI;
		std::unique_ptr<ResourceManager> m_resourceManager;
		std::unique_ptr<Resources::AssetManager> m_assetManager;
		std::unique_ptr<ProjectManager> m_projectManager;

		std::unique_ptr<Graphics::SceneRenderer> m_sceneRenderer;
		std::unique_ptr<SceneManager> m_sceneManager;
		std::unique_ptr<ScriptSystem> m_scriptSystem;
	private:
		static Application* m_appInstance;

		Window* m_window;
		Aozora::Context context;
		Window::WindowProps props;

		std::queue<std::function<void()>> m_actionQueue;
		
		void saveProject();
		void onEvent(Event& e);
	};

}

