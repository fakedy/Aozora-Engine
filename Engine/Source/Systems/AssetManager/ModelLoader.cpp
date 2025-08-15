#include "ModelLoader.h"
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include "Systems/Material.h"
#include "Systems/Texture.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include <Application.h>
#include <Systems/Logging/Logger.h>
#include <xxHash/xxhash.h>

namespace Aozora::Resources {


    const aiScene* ModelLoader::importFile(const std::string& file)
    {

        const aiScene* scene = m_importer.ReadFile(file,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_CalcTangentSpace);

        if (scene == nullptr) {
            std::cerr << "Error importing file: " << m_importer.GetErrorString() << std::endl;
        }

        return scene;

    }

    uint32_t ModelLoader::processNode(aiNode* node, const aiScene* scene, const std::string& file, Model& model, IntermediateModel& iModel) {

        uint32_t parentIndex = model.allNodes.size();

        // add new node to the model node list
        model.allNodes.emplace_back();
        Model::Node& newNode = model.allNodes[parentIndex];
        newNode.name = node->mName.C_Str();
        

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            uint32_t meshIndex = node->mMeshes[i];
            aiMesh* aiMesh = scene->mMeshes[meshIndex];
            
            // unique string for the mesh
            std::string key = file + "|" + std::to_string(meshIndex) + aiMesh->mName.C_Str();

            // check if mesh i already loaded
            if (m_importRegistry.count(key)) {
            }
            else {
                // create the mesh
                Mesh mesh = processMesh(aiMesh, scene, iModel, file);
                mesh.name = aiMesh->mName.C_Str();
                mesh.id = XXH64(key.c_str(), key.length(), 0);
                m_importRegistry[key] = mesh.id;
                iModel.meshes.push_back(mesh);
            }


            // create new nodes just for the meshes
            // can change or keep this later
            // a side effect is that if you have a parent node and a child node that contain a mesh
            // you will get 3 nodes where 2 of them are empty which is useless
            Model::Node meshNode;
            uint32_t nodeIndex = model.allNodes.size();
            model.allNodes[parentIndex].childrenNodes.push_back(nodeIndex);
            meshNode.parentNode = parentIndex; 
            meshNode.meshID = XXH64(key.c_str(), key.length(), 0);
            meshNode.hasMesh = true;
            meshNode.name = aiMesh->mName.C_Str();
            model.allNodes.push_back(meshNode);
        }

        // process the real child nodes
        for (uint32_t i = 0; i < node->mNumChildren; i++) {

            uint32_t childNode = processNode(node->mChildren[i], scene, file, model, iModel);
            model.allNodes[parentIndex].childrenNodes.push_back(childNode);
        }

        return parentIndex;
    }

    Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene, IntermediateModel& iModel, const std::string& file)
    {
        Mesh createdmesh;



        // check if the material already exist
        unsigned int mMaterialIndex = mesh->mMaterialIndex;
        // notice this might be flaws and crash with the mesh data if the name is ""
        std::string key = file + "|" + std::to_string(mMaterialIndex) + scene->mMaterials[mMaterialIndex]->GetName().C_Str();

        uint64_t materialID;
        if (m_importRegistry.count(key)) {
            materialID = m_importRegistry[key];
        }
        else {
            Material material;
            materialID = XXH64(key.c_str(), key.length(), 0);
            material.ID = materialID;
            material.name = scene->mMaterials[mMaterialIndex]->GetName().C_Str();

            aiMaterial* aimaterial = scene->mMaterials[mMaterialIndex];

            loadMaterialTextures(material, aimaterial, aiTextureType_DIFFUSE, Texture::TextureType::Texture2D, iModel);

            if (aimaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {

                loadMaterialTextures(material, aimaterial, aiTextureType_NORMALS, Texture::TextureType::Texture2D, iModel);
            }
            /* // will crash because its not finished
            else if (aimaterial->GetTextureCount(aiTextureType_HEIGHT) > 0) {
                loadMaterialTextures(material, aimaterial, aiTextureType_HEIGHT, Texture::TextureType::Texture2D, iModel);
            }
            */
            loadMaterialTextures(material, aimaterial, aiTextureType_EMISSIVE, Texture::TextureType::Texture2D, iModel);

            loadMaterialTextures(material, aimaterial, aiTextureType_AMBIENT_OCCLUSION, Texture::TextureType::Texture2D, iModel);

            loadMaterialTextures(material, aimaterial, aiTextureType_METALNESS, Texture::TextureType::Texture2D, iModel);

            loadMaterialTextures(material, aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, Texture::TextureType::Texture2D, iModel);

            m_importRegistry[key] = materialID;
            iModel.materials.push_back(material);
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

            glm::vec3 tangent;
            if (mesh->HasTangentsAndBitangents()){
                tangent.x = mesh->mTangents[v].x;
                tangent.y = mesh->mTangents[v].y;
                tangent.z = mesh->mTangents[v].z;
                vertex.Tangent = tangent;
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

        createdmesh.materialID = materialID;
        return createdmesh;
    }



    void ModelLoader::loadMaterialTextures(Material &material, aiMaterial* mat, aiTextureType type, Texture::TextureType typeName, IntermediateModel& iModel) {

        // if there are no textures we will grab the colors from the materials and create our own material
        if (mat->GetTextureCount(type) == 0) {
            switch (type) {
            case aiTextureType_DIFFUSE: {
                aiColor4D color(0.0f);
                mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                material.baseColor = glm::fvec4(color.r, color.g, color.b, color.a);
                break;
            }
            case aiTextureType_EMISSIVE: {
                aiColor4D color(0.0f);
                mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
                material.emissive = glm::fvec4(color.r, color.g, color.b, color.a);
                break;
            }
            case aiTextureType_AMBIENT_OCCLUSION: {
                aiColor3D color(0.0f);
                mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
                material.ao = color.r;
                break;
            }
            case aiTextureType_METALNESS: {
                aiColor3D color(0.0f);
                mat->Get(AI_MATKEY_METALLIC_FACTOR, color);
                material.metallic = color.r;
                break;
            }
            case aiTextureType_DIFFUSE_ROUGHNESS: {
                aiColor3D color(0.0f);
                mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, color);
                material.roughness = color.r;
                break;
            }
            }
            return;
        }
        else {

            // if we have textures we'll create textures with them...
            aiString str;
            mat->GetTexture(type, 0, &str);
            bool isSrgb = false;
            uint64_t* targetID = nullptr;
            
            switch (type)
            {
            case aiTextureType_DIFFUSE:
                isSrgb = true;
                targetID = &material.diffuseTexture;
                break;
            case aiTextureType_NORMALS:
                targetID = &material.normalTexture;
                break;
            case aiTextureType_EMISSIVE:
                isSrgb = true;
                targetID = &material.emissiveTexture;
                break;
            case aiTextureType_AMBIENT_OCCLUSION:
                targetID = &material.aoTexture;
                break;
            case aiTextureType_METALNESS:
                targetID = &material.metallicTexture;
                break;
            case aiTextureType_DIFFUSE_ROUGHNESS:
                targetID = &material.roughnessTexture;
                break;
            default:
                break;
            }

            Texture targetTexture = m_textureLoader.loadTexture(str.C_Str(), m_directory, isSrgb);
            *targetID = targetTexture.id;

            if (targetTexture.hasData) {
                targetTexture.type = typeName;
                targetTexture.path = str.C_Str();
                material.textureIDs.push_back(targetTexture.id);
                iModel.textures.push_back(targetTexture);
            }

        }
        
    }



    IntermediateModel ModelLoader::loadModel(const std::string& file)
    {
        ResourceManager& resourceManager = Application::getApplication().getResourceManager();
        Log::info(std::format("Loading model: {}", file));
        const aiScene* scene = importFile(file);
        m_directory = file.substr(0, file.find_last_of('/'));
        std::string filename = file.substr(file.find_last_of('/'), file.find_last_of('.'));
        IntermediateModel intermediateModel;
        Model loadedModel;
        loadedModel.name = filename;
        loadedModel.filePath = file;
        processNode(scene->mRootNode, scene, file, loadedModel, intermediateModel);

        intermediateModel.model = loadedModel;

        return intermediateModel;
    }
}
