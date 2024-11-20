#include "Scene.h"


namespace Aozora {
	Scene::Scene(std::shared_ptr<Renderer> renderer) : m_renderer(renderer)
	{
		m_registry = std::make_shared<entt::registry>();

	}
	void Scene::update()
	{

		auto view = m_registry->view<const CameraComponent>();
		for (const auto entity : view) {
			std::shared_ptr<Camera> camera = view.get<CameraComponent>(entity).camera;
			//renderScene(camera);
		}

	}

	void Scene::renderScene(std::shared_ptr<EditorCamera> camera)
	{
		m_renderer->clear();


		auto view = m_registry->view<const ModelComponent, TransformComponent>(); // register of all mesh components

		for (const auto entity : view) {
			auto& renderModel = view.get<ModelComponent>(entity);
			auto& transform = view.get<TransformComponent>(entity);



			m_renderer->render(m_defaultShader,transform.model, camera->getView(), camera->getProjection());

			renderModel.model->draw(m_defaultShader); // temp af
		}



		// render grid

	}

	void Scene::editorUpdate(std::shared_ptr<EditorCamera> camera)
	{

		renderScene(camera); // temp
	}
}
