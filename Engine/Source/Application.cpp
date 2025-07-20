#include "Application.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "entt/entt.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/Time.h"
#include <chrono>
#include <Systems/Project/Project.h>
namespace Aozora {

	Application* Application::m_appInstance = nullptr;

	Aozora::Application::Application(const char* title) : m_resourceManager()
	{
		// singleton
		assert(m_appInstance == nullptr);
		m_appInstance = this;


		props = Window::WindowProps(title, 1920, 1080);

		m_window = Window::create(props);
		m_renderAPI = std::unique_ptr<IrenderAPI>(IrenderAPI::create());

		m_resourceManager = std::make_unique<ResourceManager>();

		
		m_cameraSystem = std::make_unique<CameraSystem>();

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

			m_window->onUpdate(); // swap buffer

			processActions();
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = end - start;
			Time::deltaTime = elapsed.count();

		}
	}
	Scene& Application::getCurrentScene()
	{
		return *m_project->m_currentScene.get();
	}
	SceneRenderer& Application::getRenderer()
	{
		return *m_project->m_sceneRenderer.get();
	}
	void Application::queueAction(std::function<void()> func)
	{
		m_actionQueue.push(func);
	}
	void Application::processActions()
	{
		while (!m_actionQueue.empty()) {
			m_actionQueue.front()();
			m_actionQueue.pop();
		}
	}
}

