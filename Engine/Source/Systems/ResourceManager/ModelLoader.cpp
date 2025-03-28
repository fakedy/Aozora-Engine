#include "ModelLoader.h"
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include "Systems/Material.h"
#include "Systems/ResourceManager/ResourceManager.h"


namespace Aozora {


    const aiScene* ModelLoader::importFile(const std::string& file)
    {

        const aiScene* scene = m_importer.ReadFile(file,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs);

        if (scene == nullptr) {
            std::cerr << "Error importing file: " << m_importer.GetErrorString() << std::endl;
        }

        return scene;

    }

    void ModelLoader::processNode(aiNode* node, const aiScene* scene, std::vector<unsigned int>* meshVector, const std::string& file) {

        ResourceManager& resourceManager = ResourceManager::getResourceManager();

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
            std::string key = file + std::to_string(node->mMeshes[i]);
            if (resourceManager.meshLoaded(key) == -1) {
                Mesh mesh = processMesh(aiMesh, scene);
                resourceManager.m_meshPathToID[key] = resourceManager.m_nextMeshID;
                resourceManager.m_loadedMeshes[resourceManager.m_nextMeshID] = mesh;
                resourceManager.m_nextMeshID++;
                meshVector->push_back(resourceManager.meshLoaded(key));
            }
            else {
                meshVector->push_back(resourceManager.meshLoaded(key));
            }
            
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene, meshVector, file);
        }


    }

    Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        Mesh createdmesh;

        // check if the material already exist
        unsigned int mMaterialIndex = mesh->mMaterialIndex;
        Material material;

        ResourceManager& resourceManager = ResourceManager::getResourceManager();
        if(mMaterialIndex >= 0){
            //resourceManager.createMaterial(mMaterialIndex);

            aiMaterial* aimaterial = scene->mMaterials[mMaterialIndex];

            loadMaterialTextures(material, aimaterial, aiTextureType_DIFFUSE, "texture_diffuse");

            loadMaterialTextures(material, aimaterial, aiTextureType_NORMALS, "texture_normal");

            loadMaterialTextures(material, aimaterial, aiTextureType_EMISSIVE, "texture_emissive");

            loadMaterialTextures(material, aimaterial, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");

            loadMaterialTextures(material, aimaterial, aiTextureType_METALNESS, "texture_metallic");

            loadMaterialTextures(material, aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");

        }

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

            glm::vec2 texcoord;
            if (mesh->mTextureCoords[0]) {
                texcoord.x = mesh->mTextureCoords[0][v].x;
                texcoord.y = mesh->mTextureCoords[0][v].y;
                vertex.TexCoords = texcoord;
            }
            else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            // implement texture coords
            createdmesh.meshData.vertices.push_back(vertex);


        }

        // fix indices
        for (unsigned int f = 0; f < mesh->mNumFaces; f++) { // for every face in mesh i

            aiFace face = mesh->mFaces[f]; // grab the face f in mesh i
            for (unsigned int j = 0; j < face.mNumIndices; j++) // for every indice in face
                createdmesh.meshData.indices.push_back(face.mIndices[j]);
        }

        //resourceManager.createMaterial(material);
		createdmesh.material = std::make_shared<Material>(material);
        createdmesh.bufferData();
        return createdmesh;
    }



    void ModelLoader::loadMaterialTextures(Material &material, aiMaterial* mat, aiTextureType type, std::string typeName) {

        ResourceManager& resourceManager = ResourceManager::getResourceManager();

        if(mat->GetTextureCount(type) == 0){

            if (type == aiTextureType_DIFFUSE) {
                    aiColor3D color(0.0f);
                    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                    material.baseColor = glm::fvec3(color.r, color.g, color.b);
			}
			else if (type == aiTextureType_EMISSIVE) {
                aiColor3D color(0.0f);
				mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
				material.emissive = glm::fvec3(color.r, color.g, color.b);
			}
			else if (type == aiTextureType_AMBIENT_OCCLUSION) {
                aiColor3D color(0.0f);
				mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
				material.ao = color.r;
			}
			else if (type == aiTextureType_METALNESS) {
                aiColor3D color(0.0f);
				mat->Get(AI_MATKEY_METALLIC_FACTOR, color);
				material.metallic = color.r;
			}
			else if (type == aiTextureType_DIFFUSE_ROUGHNESS) {
                aiColor3D color(0.0f);
				mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, color);
				material.roughness = color.r;
			}
			return;
        }
        else {

            if (type == aiTextureType_DIFFUSE) {
                aiString str;
                mat->GetTexture(type, 0, &str);
                material.diffuseTexture.id = resourceManager.loadTexture(str.C_Str(), m_directory); // load texturefromfile here
                material.diffuseTexture.type = typeName;
                material.diffuseTexture.path = str.C_Str();
			    material.activeTextures.push_back(material.diffuseTexture);
            }
            else if (type == aiTextureType_NORMALS) {
                aiString str;
                mat->GetTexture(type, 0, &str);
                material.normalTexture.id = resourceManager.loadTexture(str.C_Str(), m_directory); // load texturefromfile here
                material.normalTexture.type = typeName;
                material.normalTexture.path = str.C_Str();
                material.activeTextures.push_back(material.normalTexture);
            }
            else if (type == aiTextureType_EMISSIVE) {
                aiString str;
                mat->GetTexture(type, 0, &str);
                material.emissiveTexture.id = resourceManager.loadTexture(str.C_Str(), m_directory); // load texturefromfile here
                material.emissiveTexture.type = typeName;
                material.emissiveTexture.path = str.C_Str();
                material.activeTextures.push_back(material.emissiveTexture);
            }
            else if (type == aiTextureType_AMBIENT_OCCLUSION) {
                aiString str;
                mat->GetTexture(type, 0, &str);
                material.aoTexture.id = resourceManager.loadTexture(str.C_Str(), m_directory); // load texturefromfile here
                material.aoTexture.type = typeName;
                material.aoTexture.path = str.C_Str();
                material.activeTextures.push_back(material.aoTexture);
            }
            else if (type == aiTextureType_METALNESS) {
                aiString str;
                mat->GetTexture(type, 0, &str);
                material.metallicTexture.id = resourceManager.loadTexture(str.C_Str(), m_directory); // load texturefromfile here
                material.metallicTexture.type = typeName;
                material.metallicTexture.path = str.C_Str();
                material.activeTextures.push_back(material.metallicTexture);
            }
            else if (type == aiTextureType_DIFFUSE_ROUGHNESS) {
                aiString str;
                mat->GetTexture(type, 0, &str);
                material.roughnessTexture.id = resourceManager.loadTexture(str.C_Str(), m_directory); // load texturefromfile here
                material.roughnessTexture.type = typeName;
                material.roughnessTexture.path = str.C_Str();
                material.activeTextures.push_back(material.roughnessTexture);
            }
        }
        
    }



    std::vector<unsigned int> ModelLoader::loadModel(const std::string& file)
    {
        const aiScene* scene = importFile(file);
        m_directory = file.substr(0, file.find_last_of('/'));
        std::cout << "\nLoading model: " << file << "\n";
        std::vector<unsigned int> meshVector;
        processNode(scene->mRootNode, scene, &meshVector, file);
        
        return meshVector;
    }
}
