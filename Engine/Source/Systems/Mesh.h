#pragma once
#include "Systems/Renderers/Shader.h"
#include "Material.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include <memory>

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
			std::vector<unsigned int> indices;
		};

		struct Texture {
			unsigned int id{};
			std::string type{};
			std::string path{};
		};

		unsigned int id;

		uint32_t materialID;

		unsigned int VAO{};
		unsigned int VBO{};
		unsigned int EBO{};

		MeshData meshData;
		void bufferData();
		void draw(Shader& shader);
		void drawGeometry();
	private:

	};
}
