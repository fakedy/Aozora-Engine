#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Systems/Mesh.h"

namespace Aozora {

	class ModelLoader {
	public:
		ModelLoader() {};

		const aiScene* importFile(const std::string& file);
		std::vector<unsigned int> loadModel(const std::string& file);


	private:

		void processNode(aiNode* node, const aiScene* scene, std::vector<unsigned int>* meshVector, const std::string& file);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		unsigned int loadTexture(const std::string path, const std::string& directory);
		void loadMaterialTextures(Material& material, aiMaterial* mat, aiTextureType type, std::string typeName);
		Assimp::Importer m_importer;
		std::string m_directory;

	};
}