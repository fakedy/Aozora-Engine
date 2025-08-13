#pragma once
#include "Systems/Renderers/Shader.h"
#include "Material.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include <memory>
#include <Systems/Serialization/SerializationGLM.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>


namespace Aozora {

	class Mesh {
	public:

		Mesh();
		

		struct Vertex {
			glm::vec3 Position{};
			glm::vec3 Normal{};
			glm::vec3 Tangent{};
			glm::vec2 TexCoords{};
		};

		struct MeshData {
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;
		};

		uint32_t id;
		std::string name{};
		uint64_t hash{};
		uint32_t materialID;

		uint32_t VAO{};
		uint32_t VBO{};
		uint32_t EBO{};

		MeshData meshData;

		bool isBuffered{ false };

		void bufferData();
		void drawGeometry();

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(id),
				CEREAL_NVP(materialID),
				CEREAL_NVP(meshData),
				CEREAL_NVP(name));
		}

	};

		template<class Archive>
		void serialize(Archive& archive, Mesh::MeshData& meshData) {
			archive(CEREAL_NVP(meshData.vertices),
				CEREAL_NVP(meshData.indices));
		}
		template<class Archive>
		void serialize(Archive& archive, Mesh::Vertex& vertex) {
			archive(CEREAL_NVP(vertex.Position),
				CEREAL_NVP(vertex.Normal),
				CEREAL_NVP(vertex.Tangent),
				CEREAL_NVP(vertex.TexCoords));
		}

}
