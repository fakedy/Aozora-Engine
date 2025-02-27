#include "Scene.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>



namespace Aozora {
	Scene::Scene(std::shared_ptr<Renderer> renderer, std::shared_ptr<entt::registry> registry) : m_renderer(renderer), m_registry(registry)
	{

	}
	void Scene::update()
	{
		// temp stuff
		auto view = m_registry->view<TransformComponent>(); // register of all mesh components
		for (const auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(glm::mat4(1.0f), transform.pos);
			// solving rotation for now
			model = glm::rotate(model, glm::radians(transform.rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(transform.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
			//model = glm::rotate(model, glm::radians(20.0f)* (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::scale(model, transform.scale);
			transform.model = model;
		}

	}

	void Scene::renderScene()
	{
		ResourceManager& resourceManager = ResourceManager::getResourceManager();
		m_renderer->clear();
		auto view = m_registry->view<const MeshComponent, TransformComponent>(); // register of all mesh components

		auto cameraView = m_registry->view<CameraComponent>();
		for (const auto cameraEntity : cameraView) { // change this we are currently rendering multiple cameras

			for (const auto entity : view) {
				auto& meshComponent = view.get<MeshComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);
                
				auto& current_camera = cameraView.get<CameraComponent>(cameraEntity);
				current_camera.m_viewPortX = m_gameViewPortX;
				current_camera.m_viewPortY = m_gameViewPortY;

				if (current_camera.isActive()) {
					m_renderer->render(m_defaultShader, transform.model, current_camera.getView(), current_camera.getProjection());

					glUniform3fv(glGetUniformLocation(m_defaultShader.ID, "cameraPos"), 1, &current_camera.getPos()[0]);
					//renderModel.model->draw(m_defaultShader); // temp af
					for (unsigned int id : meshComponent.meshIDs) {
						resourceManager.m_loadedMeshes[id].draw(m_defaultShader);
					}

				}

			}
		}
	}

	void Scene::renderEditorScene(std::shared_ptr<EditorCamera> editorCamera)
	{
		m_renderer->clear();
		ResourceManager& resourceManager = ResourceManager::getResourceManager();

		auto view = m_registry->view<const MeshComponent, TransformComponent>(); // register of all mesh components
		for (const auto entity : view) {
			auto& meshComponent = view.get<MeshComponent>(entity);
			auto& transform = view.get<TransformComponent>(entity);

			m_renderer->render(m_defaultShader, transform.model, editorCamera->getView(), editorCamera->getProjection());
			glUniform3fv(glGetUniformLocation(m_defaultShader.ID, "cameraPos"), 1, &editorCamera->getPos()[0]);

			for (unsigned int id : meshComponent.meshIDs) {
				resourceManager.m_loadedMeshes[id].draw(m_defaultShader);
			}
		}
		
	}

	void Scene::editorUpdate(std::shared_ptr<EditorCamera> editorCamera)
	{
		editorCamera->update();
		renderEditorScene(editorCamera);
		update();
	}
}
