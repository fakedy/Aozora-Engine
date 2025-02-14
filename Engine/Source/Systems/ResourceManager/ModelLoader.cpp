#include "ModelLoader.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include "Systems/Material.h"

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
        Material material;
		std::cout << "Created material\n";


        if(mesh->mMaterialIndex >= 0){
            aiMaterial* aimaterial = scene->mMaterials[mesh->mMaterialIndex];

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
		createdmesh.material = std::make_shared<Material>(material);
        createdmesh.bufferData();
        return createdmesh;
    }



    void ModelLoader::loadMaterialTextures(Material &material, aiMaterial* mat, aiTextureType type, std::string typeName) {

        // cursed if statements
        std::cout << mat->GetName().C_Str() << " Texture count: " << mat->GetTextureCount(type) << std::endl;

        if(mat->GetTextureCount(type) == 0){
            if (type == aiTextureType_DIFFUSE) {
                    aiColor3D color(0.f, 0.f, 0.f);
                    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                    material.baseColor = glm::fvec3(color.r, color.g, color.b);
			}
			else if (type == aiTextureType_EMISSIVE) {
				aiColor3D color(0.f, 0.f, 0.f);
				mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
				material.emissive = glm::fvec3(color.r, color.g, color.b);
			}
			else if (type == aiTextureType_AMBIENT_OCCLUSION) {
				aiColor3D color(0.f, 0.f, 0.f);
				mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
				material.ao = color.r;
			}
			else if (type == aiTextureType_METALNESS) {
				aiColor3D color(0.f, 0.f, 0.f);
				mat->Get(AI_MATKEY_METALLIC_FACTOR, color);
				material.metallic = color.r;
			}
			else if (type == aiTextureType_DIFFUSE_ROUGHNESS) {
				aiColor3D color(0.f, 0.f, 0.f);
				mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, color);
				material.roughness = color.r;
			}
			return;
        }

        if (type == aiTextureType_DIFFUSE) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            material.diffuseTexture.id = loadTexture(str.C_Str(), m_directory); // load texturefromfile here
            material.diffuseTexture.type = typeName;
            material.diffuseTexture.path = str.C_Str();
			material.activeTextures.push_back(material.diffuseTexture);
        }
        else if (type == aiTextureType_NORMALS) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            material.normalTexture.id = loadTexture(str.C_Str(), m_directory); // load texturefromfile here
            material.normalTexture.type = typeName;
            material.normalTexture.path = str.C_Str();
            material.activeTextures.push_back(material.normalTexture);
        }
        else if (type == aiTextureType_EMISSIVE) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            material.emissiveTexture.id = loadTexture(str.C_Str(), m_directory); // load texturefromfile here
            material.emissiveTexture.type = typeName;
            material.emissiveTexture.path = str.C_Str();
            material.activeTextures.push_back(material.emissiveTexture);
        }
        else if (type == aiTextureType_AMBIENT_OCCLUSION) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            material.aoTexture.id = loadTexture(str.C_Str(), m_directory); // load texturefromfile here
            material.aoTexture.type = typeName;
            material.aoTexture.path = str.C_Str();
            material.activeTextures.push_back(material.aoTexture);
        }
        else if (type == aiTextureType_METALNESS) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            material.metallicTexture.id = loadTexture(str.C_Str(), m_directory); // load texturefromfile here
            material.metallicTexture.type = typeName;
            material.metallicTexture.path = str.C_Str();
            material.activeTextures.push_back(material.metallicTexture);
        }
        else if (type == aiTextureType_DIFFUSE_ROUGHNESS) {
            aiString str;
            mat->GetTexture(type, 0, &str);
            material.roughnessTexture.id = loadTexture(str.C_Str(), m_directory); // load texturefromfile here
            material.roughnessTexture.type = typeName;
            material.roughnessTexture.path = str.C_Str();
            material.activeTextures.push_back(material.roughnessTexture);
        }
        return;
        
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
