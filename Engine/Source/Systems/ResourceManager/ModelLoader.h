#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Systems/Mesh.h"
#include "Systems/Model.h"

namespace Aozora {

	class ModelLoader {
	public:
		ModelLoader() {};

		const aiScene* importFile(const std::string& file);
		Model loadModel(const std::string& file);


	private:

		Model::Node* processNode(aiNode* node, const aiScene* scene, const std::string& file, Model& model);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		void loadMaterialTextures(Material& material, aiMaterial* mat, aiTextureType type, Material::TextureType typeName);
		Assimp::Importer m_importer;
		std::string m_directory;

	};
}