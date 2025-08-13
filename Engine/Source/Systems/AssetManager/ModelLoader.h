#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <vector>
#include <glm/glm.hpp>
#include <memory>
#include "Systems/Mesh.h"
#include "Systems/Model.h"
#include <Systems/AssetManager/TextureLoader.h>

namespace Aozora::Resources {



	struct IntermediateModel {

		Model model;
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		std::vector<Texture> textures;
	};

	class ModelLoader {
	public:
		ModelLoader(std::unordered_map<std::string, uint64_t>& importRegistry, TextureLoader& textureLoader)
			: m_importRegistry(importRegistry), m_textureLoader(textureLoader){};

		const aiScene* importFile(const std::string& file);
		IntermediateModel loadModel(const std::string& file);


	private:

		uint32_t processNode(aiNode* node, const aiScene* scene, const std::string& file, Model& model, IntermediateModel& iModel);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene, IntermediateModel& iModel, const std::string& file);
		void loadMaterialTextures(Material& material, aiMaterial* mat, aiTextureType type, Texture::TextureType typeName, IntermediateModel& iModel);
		Assimp::Importer m_importer;
		std::string m_directory;
		std::unordered_map<std::string, uint64_t>& m_importRegistry;
		TextureLoader& m_textureLoader;
	};
}