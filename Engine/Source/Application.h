#pragma once
#include <memory>
#include "Engine.h"
#include "Systems/Windows/Window.h"
#include "Systems/Renderers/Renderer.h"
#include "Systems/Layers/LayerStack.h"
#include "Systems/Layers/ImguiLayer.h"

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

		// ECS SYSTEM
		std::shared_ptr<entt::registry> registry;

	private:
		static Application* m_appInstance;

		Window* m_window;
		Renderer* m_renderer;

	};

}

