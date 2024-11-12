#include "Application.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "entt/entt.hpp"
#include <iostream>


namespace Aozora {

	Application* Application::m_appInstance = nullptr;

	Aozora::Application::Application(const char* title)
	{
		assert(m_appInstance == nullptr);
		m_appInstance = this;
		Window::WindowProps props = Window::WindowProps(title, 1920, 1080);

		m_window = Window::create(props);


		registry = std::make_shared<entt::registry>();
		m_renderer = Renderer::create(registry);
		m_resourceManager = new ResourceManager();


		layerStack = new LayerStack();



	}
	void Application::run()
	{

		while (!m_window->windowShouldClose()) {

			m_renderer->render(); // render call

			// TODO render after updated layers
			for (Layer* layer : *layerStack) {
				layer->onUpdate(); // update layers
			}
			m_window->onUpdate(); // swap buffer



		}
	}
}

