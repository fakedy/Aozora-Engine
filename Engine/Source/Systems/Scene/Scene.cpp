#include "Scene.h"
#include <glm/glm.hpp>


namespace Aozora {
	Scene::Scene(std::shared_ptr<Renderer> renderer) : m_renderer(renderer)
	{
		m_registry = std::make_shared<entt::registry>();

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
		m_renderer->clear();

		auto view = m_registry->view<const ModelComponent, TransformComponent>(); // register of all mesh components

		auto cameraView = m_registry->view<const CameraComponent>();
		for (const auto entity : cameraView) {
			m_activeCamera = cameraView.get<CameraComponent>(entity).camera;
			for (const auto entity : view) {
				auto& renderModel = view.get<ModelComponent>(entity);
				auto& transform = view.get<TransformComponent>(entity);

				m_renderer->render(m_defaultShader,transform.model, m_activeCamera->getView(), m_activeCamera->getProjection());

				renderModel.model->draw(m_defaultShader); // temp af
			}
		}
	}

	void Scene::renderEditorScene(std::shared_ptr<EditorCamera> editorCamera)
	{
		m_renderer->clear();

		auto view = m_registry->view<const ModelComponent, TransformComponent>(); // register of all mesh components
		for (const auto entity : view) {
			auto& renderModel = view.get<ModelComponent>(entity);
			auto& transform = view.get<TransformComponent>(entity);

			m_renderer->render(m_defaultShader, transform.model, editorCamera->getView(), editorCamera->getProjection());

			renderModel.model->draw(m_defaultShader); // temp af
		}
		
	}

	void Scene::editorUpdate(std::shared_ptr<EditorCamera> editorCamera)
	{
		editorCamera->update();
		renderEditorScene(editorCamera);
		update();
	}
}
