#pragma once
#include <entt/entt.hpp>
#include <memory>
#include "Systems/Renderers/Opengl/OpenglShader.h"
#include <Systems/ECS/Components/Components.h>
#include <Systems/Model.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <string>

namespace Aozora {

	


	class Scene {
	public:

		int m_gameViewPortX{ 1920 };
		int m_gameViewPortY{ 1080 };
	
		Scene();

		// ECS SYSTEM
		std::shared_ptr<entt::registry> m_registry;

		std::string m_snapshotData;

		uint64_t hash{ 0 };

		std::string m_sceneName{ "New Scene" };

		void update();

		entt::registry& getRegistry();

		void takeSnapshot();
		void loadSnapShot();
		void addEntity();


		entt::entity createEntityFromNodes(Model& model, Model::Node& node, entt::entity parent);
		void instantiateEntity(uint64_t hash);

		void deleteEntity(const entt::entity entity);

		// returns a vector of entities
		std::vector<entt::entity> getSceneHierarchyEntities();

		std::string getEntityName(entt::entity entity);

		std::vector<entt::entity>& getEntityChildren(entt::entity entity);

		entt::entity& getEntityParent(entt::entity entity);

		void makeTransformDirty(entt::entity entity);

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(m_gameViewPortX),
				CEREAL_NVP(m_gameViewPortY),
				CEREAL_NVP(hash),
				CEREAL_NVP(m_snapshotData),
				CEREAL_NVP(m_sceneName));
		}



	private:

		void updateTransform(entt::entity entity, const glm::mat4& model);

		
	};


}
