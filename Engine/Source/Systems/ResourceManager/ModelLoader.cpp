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
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs);

        if (scene == nullptr) {
            std::cout << "assimp import fail\n";
        }

        return scene;

    }

    void ModelLoader::processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>* meshVector) {


        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshVector->push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene, meshVector);
        }


    }

    Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        Mesh createdmesh;

        for (uint32_t v = 0; v < mesh->mNumVertices; v++) {

            Mesh::Vertex vertex;

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

            glm::vec3 texcoord;
            // implement texture coords
            createdmesh.meshData.vertices.push_back(vertex);

        }

        // fix indices
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) { // for every face in mesh i

            aiFace face = mesh->mFaces[f]; // grab the face f in mesh i
            for (unsigned int j = 0; j < face.mNumIndices; j++) // for every indice in face
                createdmesh.meshData.indices.push_back(face.mIndices[j]);
        }
        createdmesh.bufferData();
        return createdmesh;
    }

    std::vector<Mesh> ModelLoader::loadModel(const std::string& file)
    {
        const aiScene* scene = importFile(file);
        std::vector<Mesh> meshVector;
        processNode(scene->mRootNode, scene, &meshVector);
        
        return meshVector;
    }
}
