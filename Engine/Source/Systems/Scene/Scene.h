#pragma once
#include <entt/entt.hpp>
#include <memory>
#include "Systems/Renderers/Opengl/OpenglShader.h"
#include "..\ResourceManager\ResourceManager.h"
#include <string>
#include <Systems/ECS/Components/Components.h>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <Systems/ECS/Components/Components.h>


namespace Aozora {

	


	class Scene {
	public:

		int m_gameViewPortX{ 1920 };
		int m_gameViewPortY{ 1080 };
	
		Scene();

		// ECS SYSTEM
		std::shared_ptr<entt::registry> m_registry;

		std::stringstream data;

		const char* m_sceneName{ "New Scene" };

		void update();

		entt::registry& getRegistry();

		void takeSnapshot();
		void loadSnapShot();
		void addEntity();


		entt::entity createEntityFromNodes(Model::Node* node, entt::entity parent);
		void instantiateEntity(std::string name);

		void deleteEntity(const entt::entity entity);

		// returns a vector of entities
		std::vector<entt::entity> getSceneHierarchyEntities();

		std::string getEntityName(entt::entity entity);

		std::vector<entt::entity>& getEntityChildren(entt::entity entity);

		entt::entity& getEntityParent(entt::entity entity);


		void makeTransformDirty(entt::entity entity);

	private:

		void updateTransform(entt::entity entity, const glm::mat4& model);

		
	};
}
