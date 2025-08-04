#include <AozoraAPI/Aozora.h>
#include <Systems/Scene/Scene.h>
#include <Application.h>

#include "Systems/ECS/Components/Components.h"

namespace Aozora {


	void SceneAPI::addEntity() {


		Scene& currentScene = Application::getApplication().getCurrentScene();
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		entt::registry& registry = currentScene.getRegistry();
		const auto entity = registry.create();

		// one entity per mesh
		registry.emplace<Aozora::NameComponent>(entity).name = "Entity";
		registry.emplace<Aozora::TagComponent>(entity);
		registry.emplace<Aozora::TransformComponent>(entity);
		registry.emplace<Aozora::RelationComponent>(entity);

	}

	void SceneAPI::deleteEntity(const entt::entity entity) {

		Scene& currentScene = Application::getApplication().getCurrentScene();
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		entt::registry& registry = currentScene.getRegistry();

		registry.destroy(entity);

		// TODO DELETE THE RELATIONS OR FACE DEATH


	}

	// returns a vector of entities
	std::vector<entt::entity> SceneAPI::getSceneHierarchyEntities()
	{

		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();

		// everything but editor related entities
		// want to sort this by name first
		// https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system/465d90e0f5961adc460cd9d1e9358370987fbcd3#sorting-is-it-possible
		auto view = registry.view<NameComponent>(entt::exclude<EditorEntityTag>);

		return { view.begin(), view.end() };
	}

	std::string SceneAPI::getEntityName(entt::entity entity)
	{
		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();
		auto target = registry.get<NameComponent>(entity);
		return target.name;
	}

	std::vector<entt::entity>& SceneAPI::getEntityChildren(entt::entity entity)
	{
		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();
		auto& relationComponent = registry.get<RelationComponent>(entity);

		return relationComponent.children;

	}

	entt::entity& SceneAPI::getEntityParent(entt::entity entity)
	{

		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();
		auto& relationComponent = registry.get<RelationComponent>(entity);

		return relationComponent.parent;
		
	}

	void SceneAPI::makeTransformDirty(entt::entity entity)
	{

		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();
		auto& transformComponent = registry.get<TransformComponent>(entity);
		transformComponent.isDirty = true;

		auto& parent = SceneAPI::getEntityParent(entity);
		if (parent != entt::null) {
			makeTransformDirty(SceneAPI::getEntityParent(entity));
		}

	}

	void SceneAPI::takeSnapshot()
	{
		Application::getApplication().getCurrentScene().takeSnapshot();
	}

	void SceneAPI::loadSnapshot()
	{
		Application::getApplication().getCurrentScene().loadSnapShot();
	}











}
