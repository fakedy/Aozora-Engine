#include "ModelLoader.h"
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include "Systems/Material.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include <Application.h>


namespace Aozora {


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

Model::Node* ModelLoader::processNode(aiNode* node, const aiScene* scene, const std::string& file, Model& model) {

    ResourceManager& resourceManager = Application::getApplication().getResourceManager();

    Model::Node* newNode = new Model::Node();
    newNode->name = node->mName.C_Str();
    // set the model origin node
    if (model.originNode == nullptr) {
        model.originNode = newNode;
    }

    // add new node to the model node list
    model.allNodes.push_back(newNode);


    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        std::string key = file + std::to_string(node->mMeshes[i]);

        uint32_t meshID;
        // if mesh isnt already loaded
        if (resourceManager.meshLoaded(key) == -1) {
            Mesh mesh = processMesh(aiMesh, scene);
            resourceManager.m_meshPathToID[key] = resourceManager.m_nextMeshID;
            resourceManager.m_loadedMeshes[resourceManager.m_nextMeshID] = mesh;
            meshID = resourceManager.m_nextMeshID; // use new mesh ID
            resourceManager.m_nextMeshID++;
        }
        else {
            meshID = resourceManager.meshLoaded(key); // use the already loaded mesh ID
        }

        // all mesh nodes will be children to the parent node
        Model::Node* meshNode = new Model::Node();
        newNode->childrenNodes.push_back(meshNode);
        meshNode->parentNode = newNode;
        meshNode->meshID = meshID;
        meshNode->hasMesh = true;
        meshNode->name = aiMesh->mName.C_Str();
        model.allNodes.push_back(meshNode);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {

        Model::Node* childNode = processNode(node->mChildren[i], scene, file, model);
        newNode->childrenNodes.push_back(childNode);
        childNode->parentNode = newNode;
    }

    return newNode;

}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    Mesh createdmesh;

    // check if the material already exist
    unsigned int mMaterialIndex = mesh->mMaterialIndex;
    Material material;

    ResourceManager& resourceManager = Application::getApplication().getResourceManager();
    if (mMaterialIndex >= 0) {

        aiMaterial* aimaterial = scene->mMaterials[mMaterialIndex];

        loadMaterialTextures(material, aimaterial, aiTextureType_DIFFUSE, Material::TextureType::DIFFUSE);

        loadMaterialTextures(material, aimaterial, aiTextureType_NORMALS, Material::TextureType::NORMAL);

        loadMaterialTextures(material, aimaterial, aiTextureType_EMISSIVE, Material::TextureType::EMISSIVE);

        loadMaterialTextures(material, aimaterial, aiTextureType_AMBIENT_OCCLUSION, Material::TextureType::AO);

        loadMaterialTextures(material, aimaterial, aiTextureType_METALNESS, Material::TextureType::METALLIC);

        loadMaterialTextures(material, aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, Material::TextureType::ROUGHNESS);

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

        // creating material
        createdmesh.materialID = resourceManager.m_nextMaterialID;
        material.name = scene->mMaterials[mMaterialIndex]->GetName().C_Str();
        resourceManager.m_loadedmaterials[resourceManager.m_nextMaterialID] = material;
        resourceManager.m_nextMaterialID++;
        return createdmesh;
    }



    void ModelLoader::loadMaterialTextures(Material &material, aiMaterial* mat, aiTextureType type, Material::TextureType typeName) {

        ResourceManager& resourceManager = Application::getApplication().getResourceManager();

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

            Material::Texture* targetTexture = nullptr;
            switch (type)
            {
            case aiTextureType_DIFFUSE:
                targetTexture = &material.diffuseTexture;
                break;
            case aiTextureType_NORMALS:
                targetTexture = &material.normalTexture;
                break;
            case aiTextureType_EMISSIVE:
                targetTexture = &material.emissiveTexture;
                break;
            case aiTextureType_AMBIENT_OCCLUSION:
                targetTexture = &material.aoTexture;
                break;
            case aiTextureType_METALNESS:
                targetTexture = &material.metallicTexture;
                break;
            case aiTextureType_DIFFUSE_ROUGHNESS:
                targetTexture = &material.roughnessTexture;
                break;
            default:
                break;
            }

            if (targetTexture) {

                aiString str;
                mat->GetTexture(type, 0, &str);
                targetTexture->id = resourceManager.loadTexture(str.C_Str(), m_directory);
                targetTexture->type = typeName;
                targetTexture->path = str.C_Str();
                material.activeTextures.push_back(*targetTexture);
            }

        }
        
    }



    Model ModelLoader::loadModel(const std::string& file)
    {
        ResourceManager& resourceManager = Application::getApplication().getResourceManager();
        const aiScene* scene = importFile(file);
        m_directory = file.substr(0, file.find_last_of('/'));
        std::cout << "\nLoading model: " << file << "\n";
        std::string filename = file.substr(file.find_last_of('/'), file.find_last_of('.'));
        Model loadedModel;
        loadedModel.name = filename;
        loadedModel.filePath = file;
        processNode(scene->mRootNode, scene, file, loadedModel);

        return loadedModel;
    }
}
