#include "Application.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "entt/entt.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/Time.h"
#include <chrono>


namespace Aozora {

	Application* Application::m_appInstance = nullptr;

	Aozora::Application::Application(const char* title) : m_resourceManager(ResourceManager::getResourceManager())
	{
		// singleton
		assert(m_appInstance == nullptr);
		m_appInstance = this;


		Window::WindowProps props = Window::WindowProps(title, 1920, 1080);

		m_window = Window::create(props);
		m_registry = std::make_shared<entt::registry>();
		m_cameraSystem = std::make_unique<CameraSystem>(m_registry);
		m_renderer = std::shared_ptr<Renderer>(Renderer::create(props));
		m_currentScene = std::make_shared<Scene>(m_renderer, m_registry);

		/*
		std::cout << "Material count: " << m_resourceManager.m_loadedmaterials.size() << "\n";
		std::cout << "Texture count: " << m_resourceManager.m_loadedTextures.size() << "\n";
		std::cout << "Mesh count: " << m_resourceManager.m_loadedMeshes.size() << "\n";
		*/
		layerStack = new LayerStack();

	}

	void Application::run()
	{

		while (!m_window->windowShouldClose()) {
			auto start = std::chrono::high_resolution_clock::now();


			// TODO render after updated layers
			for (Layer* layer : *layerStack) {
				layer->onUpdate(); // update layers
			}

			m_cameraSystem->update(); 



			m_window->onUpdate(); // swap buffer


			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = end - start;
			Time::deltaTime = elapsed.count();

		}
	}
}

