#pragma once
#include "Systems/Renderers/Shader.h"
#include "glm/glm.hpp"
#include <vector>

namespace Aozora {

	class Mesh {
	public:

		Mesh();

		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		struct MeshData {
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
		};

		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		

		MeshData meshData;
		void bufferData();
		void draw();
	private:







	};
}
