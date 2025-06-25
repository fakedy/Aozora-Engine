#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <Application.h>



namespace Aozora {
	Scene::Scene()
	{
		m_registry = std::make_shared<entt::registry>();

	}
	void Scene::update()
	{
		// essentially a system that act on transforms
		auto view = m_registry->view<TransformComponent>(); // register of all transform components
		for (const auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(glm::mat4(1.0f), transform.pos);
			glm::quat rotation = glm::quat(glm::radians(transform.rot));
			model = model * glm::mat4_cast(rotation);
			model = glm::scale(model, transform.scale);
			transform.model = model;
		}

		/*
		


			editorCamera->update();
			renderEditorScene(editorCamera);


		*/
	}



	entt::registry& Scene::getRegistry()
	{
		return *m_registry;
	}
}
