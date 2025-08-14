#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <Application.h>
#include <Systems/Logging/Logger.h>
#include <Systems/AssetManager/AssetManager.h>

namespace Aozora {
	Scene::Scene()
	{
		m_registry = std::make_shared<entt::registry>();

	}
	void Scene::update()
	{

		// recursive update, going from parents and updating their children i guess.
		const std::vector<entt::entity>& view = getSceneHierarchyEntities();

		auto transforms = m_registry->view<TransformComponent>();
		for (entt::entity entity : view) {
			auto& transform = transforms.get<TransformComponent>(entity);

			if (transform.isDirty && getEntityParent(entity) == entt::null) {
				updateTransform(entity, glm::mat4(1.0f));
			}
		}

	}

	entt::registry& Scene::getRegistry()
	{
		return *m_registry;
	}

	void Scene::takeSnapshot()
	{
		// TODO make it exclude the editor camera
		data.str("");
		data.clear();
		cereal::BinaryOutputArchive output(data); // what a most vexing parse
		entt::snapshot snapshot{ *m_registry };
		snapshot.get<entt::entity>(output)
			.get<NameComponent>(output)
			.get<TransformComponent>(output)
			.get<CameraComponent>(output)
			.get<EditorEntityTag>(output)
			.get<LightComponent>(output)
			.get<MeshComponent>(output)
			.get<RelationComponent>(output)
			.get<RigidBodyComponent>(output)
			.get<ScriptComponent>(output)
			.get<TagComponent>(output)
			.get<SkyboxComponent>(output);
	}

	void Scene::loadSnapShot()
	{
		// check if we have any data
		if (data.str().empty()) {
			return;
		}

		// reset read position
		data.seekg(0);

		cereal::BinaryInputArchive input(data);
		m_registry->clear();
		entt::snapshot_loader loader{ *m_registry };
		// need archive
		loader.get<entt::entity>(input)
			.get<NameComponent>(input)
			.get<TransformComponent>(input)
			.get<CameraComponent>(input)
			.get<EditorEntityTag>(input)
			.get<LightComponent>(input)
			.get<MeshComponent>(input)
			.get<RelationComponent>(input)
			.get<RigidBodyComponent>(input)
			.get<ScriptComponent>(input)
			.get<TagComponent>(input)
			.get<SkyboxComponent>(input).orphans();
		

		data.clear();

	}

	void Scene::addEntity()
	{
		const auto entity = m_registry->create();

		// one entity per mesh
		m_registry->emplace<Aozora::NameComponent>(entity).name = "Entity";
		m_registry->emplace<Aozora::TagComponent>(entity);
		m_registry->emplace<Aozora::TransformComponent>(entity);
		m_registry->emplace<Aozora::RelationComponent>(entity);
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
		
		const std::vector<entt::entity>& children = getEntityChildren(entity);
		transform.isDirty = false;
		// update children
		for (const auto entity : children) {
			updateTransform(entity, transform.model);
		}

	}
	entt::entity Scene::createEntityFromNodes(Model& model, Model::Node& node, entt::entity parent)
	{

		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		const auto entity = m_registry->create();

		m_registry->emplace<Aozora::NameComponent>(entity).name = node.name.c_str();
		m_registry->emplace<Aozora::TagComponent>(entity);
		m_registry->emplace<Aozora::TransformComponent>(entity);
		if (node.hasMesh) {
			// add a mesh component to the entity with the id of the mesh
			auto& meshComp = m_registry->emplace<Aozora::MeshComponent>(entity);
			meshComp.meshID = node.meshID;
			meshComp.materialID = resourceManager.m_loadedMeshes.at(node.meshID).materialID;

			// this is for the deferred rendering buffer, should take a look at this later
			if (!resourceManager.m_loadedMeshes.at(node.meshID).isBuffered) {
				
				EntityCreatedWithMeshEvent* event = new EntityCreatedWithMeshEvent(entity, node.meshID, this);
				EventDispatcher::dispatch(event);
			}
		}

		Aozora::RelationComponent& relationComponent = m_registry->emplace<Aozora::RelationComponent>(entity, parent);

		for (uint32_t childNode : node.childrenNodes) {

			entt::entity childEntity = createEntityFromNodes(model, model.allNodes[childNode], entity);
			relationComponent.children.push_back(childEntity);

		}
		return entity;
	}
	void Scene::instantiateEntity(uint64_t hash)
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		Log::info(std::format("Instantiating {}", hash));

		resourceManager.loadModel(hash);
		Model& model = resourceManager.m_loadedModels.at(hash);

		// could totally cache the entity too so I dont have to do this node work
		createEntityFromNodes(model, model.allNodes[0], entt::null);

	}

	void Scene::deleteEntity(const entt::entity entity) {

		m_registry->destroy(entity);

	}

	// returns a vector of entities
	std::vector<entt::entity> Scene::getSceneHierarchyEntities()
	{
		// everything but editor related entities
		// want to sort this by name first
		// https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system/465d90e0f5961adc460cd9d1e9358370987fbcd3#sorting-is-it-possible
		auto view = m_registry->view<NameComponent>(entt::exclude<EditorEntityTag>);

		return { view.begin(), view.end() };
	}

	std::string Scene::getEntityName(entt::entity entity)
	{
		auto target = m_registry->get<NameComponent>(entity);
		return target.name;
	}

	std::vector<entt::entity>& Scene::getEntityChildren(entt::entity entity)
	{
		auto& relationComponent = m_registry->get<RelationComponent>(entity);

		return relationComponent.children;

	}

	entt::entity& Scene::getEntityParent(entt::entity entity)
	{

		
		auto& relationComponent = m_registry->get<RelationComponent>(entity);

		return relationComponent.parent;

	}
	void Scene::makeTransformDirty(entt::entity entity)
	{

		
		auto& transformComponent = m_registry->get<TransformComponent>(entity);
		transformComponent.isDirty = true;

		auto& parent = getEntityParent(entity);
		if (parent != entt::null) {
			makeTransformDirty(getEntityParent(entity));
		}

	}
}
