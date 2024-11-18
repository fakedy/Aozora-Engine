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
		ModelLoader();

		const aiScene* importFile(const std::string& file);
		std::vector<Mesh> loadModel(const std::string& file);


	private:

		void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>* meshVector);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		static ModelLoader* m_modelLoader;
		Assimp::Importer m_importer;

	};
}