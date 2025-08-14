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
		m_sceneManager = std::make_unique<SceneManager>();
		m_sceneRenderer = std::make_unique<Graphics::SceneRenderer>(m_renderAPI.get());
		m_assetManager = std::make_unique<Resources::AssetManager>();
		m_resourceManager = std::make_unique<ResourceManager>(*m_assetManager.get());
		m_projectManager = std::make_unique<ProjectManager>(*m_sceneManager.get());
		
		
		m_cameraSystem = std::make_unique<CameraSystem>();

		layerStack = new LayerStack();

		context.renderAPI = m_renderAPI.get();
		context.sceneManager = m_sceneManager.get();
		context.sceneRenderer = m_sceneRenderer.get();
		context.resourcemanager = m_resourceManager.get();
		context.projectManager = m_projectManager.get();
		context.assetManager = m_assetManager.get();

	
		EventDispatcher::subscribe(EventType::CreateProjectRequest, [this](Event& e) {
			this->onEvent(e);
			});
		EventDispatcher::subscribe(EventType::SaveProjectRequest, [this](Event& e) {
			this->onEvent(e);
			});

	}

	void Application::run()
	{

		while (!m_window->windowShouldClose()) {
			auto start = std::chrono::high_resolution_clock::now();
			processActions(); // process non event based queued functions
			EventDispatcher::flush(); // goes through events queued for processing

			// TODO render after updated layers
			for (Layer* layer : *layerStack) {
				layer->onUpdate(context); // update layers
			}

			m_window->onUpdate(); // swap buffer

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> elapsed = end - start;
			Time::deltaTime = elapsed.count();

		}
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

	void Application::createProject()
	{
		m_resourceManager->clearResources();
		m_sceneManager->clearScenes();

		// init new project directory
		m_assetManager->createProject("MyProject");

		// create the project object (for future settings?)
		m_projectManager->createProject("MyProject");

		uint32_t sceneID = m_sceneManager->createScene();
		Scene* scene = m_sceneManager->getScene(sceneID);
		// creating the editor camera
		// will be invisible to the scene graph
		Aozora::CameraComponent* cameraComponent;
		Aozora::TransformComponent* transformComponent;
		auto& registry = scene->getRegistry();
		const entt::entity editorCameraEntity = registry.create();
		transformComponent = &registry.emplace<Aozora::TransformComponent>(editorCameraEntity);
		cameraComponent = &registry.emplace<Aozora::CameraComponent>(editorCameraEntity);
		registry.emplace<Aozora::EditorEntityTag>(editorCameraEntity);


		
		// create skybox
		const entt::entity skyboxEntity = registry.create();
		registry.emplace_or_replace<EditorEntityTag>(skyboxEntity);


		uint64_t skyboxID = m_resourceManager->loadSkybox(m_assetManager->createSkybox());


		registry.emplace_or_replace<SkyboxComponent>(skyboxEntity).id = skyboxID;

		
		m_sceneRenderer->updatePrimaryScene(*scene);

	}

	void Application::saveProject()
	{
	}

	// move to editor
	void Application::onEvent(Event& e)
	{

		switch (e.getEventType())
		{
		case(Aozora::EventType::CreateProjectRequest): 
			createProject();
			break;

		case(Aozora::EventType::SaveProjectRequest):
			
			break;
		
		default:
			break;
		}

	}

}

