#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <Application.h>


namespace Aozora {
	Scene::Scene()
	{
		m_registry = std::make_shared<entt::registry>();

		// create skybox
		/*
		* Well the good thing is that its in the registry so its easy to query, the bad thing is that its tied to the scene...
		*/
		const entt::entity skyboxEntity = m_registry->create();
		m_registry->emplace_or_replace<EditorEntityTag>(skyboxEntity);
		std::vector<std::string> paths = {
			"Resources/cubemap/px.hdr",
			"Resources/cubemap/nx.hdr",
			"Resources/cubemap/py.hdr",
			"Resources/cubemap/ny.hdr",
			"Resources/cubemap/pz.hdr",
			"Resources/cubemap/nz.hdr"
		};
		SkyboxTextures data = ResourcesAPI::loadSkybox(paths);
		m_registry->emplace_or_replace<SkyboxComponent>(skyboxEntity).data = data;
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
