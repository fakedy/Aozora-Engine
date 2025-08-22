#pragma once
#include <string>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <Systems/Serialization/SerializationGLM.h>

namespace Aozora {

	class Model {
	public:
		// for preserving the hierarchal data
		class Node {
		public:
			uint32_t parentNode{0};
			std::vector<uint32_t> childrenNodes{};
			glm::mat4 transform = glm::mat4(1.0f);
			uint64_t meshID; // hash
			bool hasMesh{ false };
			std::string name{};
		};
		
		std::string name{};
		uint64_t hash{};
		std::string filePath{};
		std::vector<Node> allNodes;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(name),
				CEREAL_NVP(filePath),
				CEREAL_NVP(allNodes));
		}
	};


	template<class Archive>
	void serialize(Archive& archive, Model::Node& node) {
		archive(CEREAL_NVP(node.parentNode),
			CEREAL_NVP(node.childrenNodes),
			CEREAL_NVP(node.meshID),
			CEREAL_NVP(node.hasMesh),
			CEREAL_NVP(node.name),
			CEREAL_NVP(node.transform));
	}

}


