#include "Application.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "entt/entt.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


namespace Aozora {

	Application* Application::m_appInstance = nullptr;

	Aozora::Application::Application(const char* title)
	{
		// singleton
		assert(m_appInstance == nullptr);
		m_appInstance = this;


		Window::WindowProps props = Window::WindowProps(title, 1920, 1080);

		m_window = Window::create(props);

		m_currentScene = std::make_shared<Scene>();


		m_renderer = Renderer::create(m_currentScene, props);
		m_resourceManager = new ResourceManager();

		layerStack = new LayerStack();

	}

	void Application::run()
	{

		while (!m_window->windowShouldClose()) {

			m_renderer->render(); // render call

			// temp stuff
			auto view = m_currentScene.get()->registry->view<TransformComponent>(); // register of all mesh components
			for (const auto entity : view) {
				auto& transform = view.get<TransformComponent>(entity);
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(glm::mat4(1.0f), transform.pos);
				// solving rotation for now
				model = glm::rotate(model, glm::radians(transform.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(transform.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(transform.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::scale(model, transform.scale);
				transform.model = model;
			}

			// TODO render after updated layers
			for (Layer* layer : *layerStack) {
				layer->onUpdate(); // update layers
			}
			m_window->onUpdate(); // swap buffer

		}
	}
}

