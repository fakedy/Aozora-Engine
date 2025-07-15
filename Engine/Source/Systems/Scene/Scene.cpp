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

		// recursive update, going from parents and updating their children i guess.
		const std::vector<entt::entity>& view = Aozora::SceneAPI::getSceneHierarchyEntities();

		auto transforms = m_registry->view<TransformComponent>();
		for (entt::entity entity : view) {
			auto& transform = transforms.get<TransformComponent>(entity);

			if (transform.isDirty && Aozora::SceneAPI::getEntityParent(entity) == entt::null) {
				updateTransform(entity, glm::mat4(1.0f));
			}
		}

	}

	entt::registry& Scene::getRegistry()
	{
		return *m_registry;
	}

	void Scene::updateTransform(entt::entity entity , const glm::mat4& model)
	{
		auto view = m_registry->view<TransformComponent>();
		
		auto& transform = view.get<TransformComponent>(entity);

		glm::mat4 tempModel = glm::mat4(1.0f);
		tempModel = glm::translate(glm::mat4(1.0f), transform.pos);
		glm::quat rotation = glm::quat(glm::radians(transform.rot));
		tempModel = tempModel * glm::mat4_cast(rotation);
		tempModel = glm::scale(tempModel, transform.scale);
		tempModel = model * tempModel;
		transform.model = tempModel;
		
		const std::vector<entt::entity>& children = Aozora::SceneAPI::getEntityChildren(entity);
		transform.isDirty = false;
		// update children
		for (const auto entity : children) {
			updateTransform(entity, transform.model);
		}

	}
}
