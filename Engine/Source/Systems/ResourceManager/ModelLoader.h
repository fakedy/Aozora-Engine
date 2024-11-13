#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <vector>
#include <glm/glm.hpp>
#include <memory>

namespace Aozora {

	class ModelLoader {
	public:
		ModelLoader();


		struct Vertex {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
		};

		struct MeshData {
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;
		};

		const aiScene* importFile(const std::string& file);
		std::unique_ptr<MeshData> loadModel(const std::string& file);


	private:

		static ModelLoader* m_modelLoader;
		Assimp::Importer m_importer;

	};
}