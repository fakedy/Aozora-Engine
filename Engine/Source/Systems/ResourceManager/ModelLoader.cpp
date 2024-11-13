#include "ModelLoader.h"
#include <iostream>


namespace Aozora {


    ModelLoader* ModelLoader::m_modelLoader = nullptr;

    ModelLoader::ModelLoader()
    {
	    if (m_modelLoader == nullptr) {
		    m_modelLoader = this;
	    }
    }

    const aiScene* ModelLoader::importFile(const std::string& file)
    {

        const aiScene* scene = m_importer.ReadFile(file,
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);

        if (scene == nullptr) {
            std::cout << "assimp import fail\n";
        }

        return scene;

    }

    std::unique_ptr<ModelLoader::MeshData> ModelLoader::loadModel(const std::string& file)
    {
        const aiScene* scene = importFile(file);
        MeshData data;
        for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[i];
            for (uint32_t v = 0; v < mesh->mNumVertices; v++) {

                Vertex vertex;

                glm::vec3 position;
                position.x = mesh->mVertices[v].x;
                position.y = mesh->mVertices[v].y;
                position.z = mesh->mVertices[v].z;
                vertex.Position = position;

                glm::vec3 normal;
                if (mesh->HasNormals()) {
                    normal.x = mesh->mNormals[v].x;
                    normal.y = mesh->mNormals[v].y;
                    normal.z = mesh->mNormals[v].z;
                    vertex.Normal = normal;
                }
                else {
                    normal = glm::vec3(0.0f);
                    vertex.Normal = normal;
                }

                glm::vec3 texcoord;
                // implement texture coords
                data.vertices.push_back(vertex);

            }

            // fix indices
            for (uint32_t k = 0; k < scene->mMeshes[i]->mNumFaces; k++) {
                aiFace face = scene->mMeshes[i]->mFaces[k];
                for (uint32_t j = 0; j < face.mNumIndices; j++)
                    data.indices.push_back(face.mIndices[j]);
            }
        }


        return std::make_unique<ModelLoader::MeshData>(data);
    }
}
