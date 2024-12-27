#include "ModelLoader.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

        if(mesh->mMaterialIndex >= 0){
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            createdmesh.textures.insert(createdmesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Mesh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
            createdmesh.textures.insert(createdmesh.textures.end(), normalMaps.begin(), normalMaps.end());

            std::vector<Mesh::Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
            createdmesh.textures.insert(createdmesh.textures.end(), emissiveMaps.begin(), emissiveMaps.end());

            std::vector<Mesh::Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");
            createdmesh.textures.insert(createdmesh.textures.end(), ambientMaps.begin(), ambientMaps.end());

            std::vector<Mesh::Texture> metallicMaps = loadMaterialTextures(material, aiTextureType_METALNESS, "texture_metallic");
            createdmesh.textures.insert(createdmesh.textures.end(), metallicMaps.begin(), metallicMaps.end());

            std::vector<Mesh::Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
            createdmesh.textures.insert(createdmesh.textures.end(), roughnessMaps.begin(), roughnessMaps.end());
            
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
        createdmesh.bufferData();
        return createdmesh;
    }



    std::vector<Mesh::Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
        std::vector<Mesh::Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            Mesh::Texture texture;
            texture.id = loadTexture(str.C_Str(), m_directory); // load texturefromfile here
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
        }
        return textures;
    }

    unsigned int ModelLoader::loadTexture(const std::string path, const std::string& directory) // TODO move to separate and separate opengl implementation
    {
        std::string filename = std::string(path);
        filename = directory + "/" + filename;
        std::cout << "Loading Texture: " << filename << "\n";
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        int width, height, nrChannels;

        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            if (nrChannels == 4) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
            else {

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            }
        }
        else {
            std::cerr << "texture load failed\n";
        }
        stbi_image_free(data);

        return texture;
    }




    std::vector<Mesh> ModelLoader::loadModel(const std::string& file)
    {
        const aiScene* scene = importFile(file);
        m_directory = file.substr(0, file.find_last_of('/'));
        std::vector<Mesh> meshVector;
        processNode(scene->mRootNode, scene, &meshVector);
        
        return meshVector;
    }
}
