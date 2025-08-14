#include "ResourceManager.h"
#include <Systems/Logging/Logger.h>
#include <variant>

namespace Aozora {


    const void ResourceManager::loadModel(uint64_t hash)
    {
       
       // if it is not cached
       if (m_loadedModels.find(hash) == m_loadedModels.end()) {
           const Model& model = m_assetManager.loadModelFromDisk(hash);

           for (const auto& node : model.allNodes) {
               if (node.hasMesh) {
                   Mesh mesh = m_assetManager.loadMeshFromDisk(node.meshID);
                   m_loadedMeshes[node.meshID] = mesh;
                   m_loadedmaterials[mesh.materialID] = m_assetManager.loadMaterialFromDisk(mesh.materialID);

                   for (uint64_t texID : m_loadedmaterials[mesh.materialID].textureIDs) {
                       loadTexture(texID);
                   }
               }
           }
           m_loadedModels.emplace(hash, model);

       }
       else {
           Log::info(std::format("Using cache for model: {}", hash));
       }
    }

    // opengl dependent code for loading texture
    uint64_t ResourceManager::loadTexture(uint64_t hash)
    {
        
        unsigned int texture = 0;

        // Checks if the texture is already in memory if so then reference it by id
        int isAlreadyLoaded = textureLoaded(hash);
        if (isAlreadyLoaded != -1) {

            return isAlreadyLoaded;
        }

        Texture tex = m_assetManager.loadTextureFromDisk(hash);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        GLenum internalFormat;
        GLenum sourceFormat;
        GLenum sourceType;

        std::visit([&](auto&& data) {

            using VecType = std::decay_t<decltype(data[0])>;
            using PixelType = typename VecType::value_type;

            // basically check if they are the same type
            if (std::is_same_v<PixelType, float>) {
                internalFormat = (tex.nrChannels == 4) ? GL_RGBA16F : GL_RGB16F;
                sourceFormat = (tex.nrChannels == 4) ? GL_RGBA : GL_RGB;
                sourceType = GL_FLOAT;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex.width, tex.height, 0, sourceFormat, sourceType, data[0].data());
            }
            else {

                internalFormat = (tex.nrChannels == 4) ? GL_SRGB8_ALPHA8 : GL_SRGB8;
                sourceFormat = (tex.nrChannels == 4) ? GL_RGBA : GL_RGB;
                sourceType = GL_UNSIGNED_BYTE;

                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex.width, tex.height, 0, sourceFormat, sourceType, data[0].data());
            }
        }, tex.dataVector);


        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        uint64_t handle = glGetTextureHandleARB(texture);
        glMakeTextureHandleResidentARB(handle);
        tex.handle = handle;
        m_loadedTextures[hash] = tex;
        Log::info(std::format("Created texture with ID: {}", texture));

        return texture;
        
    }

    uint64_t ResourceManager::loadCubemap(uint64_t hash)
    {

        Texture texture = m_assetManager.loadTextureFromDisk(hash);

        uint32_t textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        GLenum internalFormat;
        GLenum sourceFormat;
        GLenum sourceType;

        std::visit([&](auto&& data) {

            using VecType = std::decay_t<decltype(data[0])>;
            using PixelType = typename VecType::value_type;
            int width = texture.width;
            int height = texture.height;
            // basically check if they are the same type
            if (std::is_same_v<PixelType, float>) {
                if (texture.nrChannels == 4) {
                    internalFormat = GL_RGBA16F;
                    sourceFormat = GL_RGBA;
                    sourceType = GL_FLOAT;
                }
                else {
                    internalFormat = GL_RGB16F;
                    sourceFormat = GL_RGB;
                    sourceType = GL_FLOAT;
                }
            }
            else {
                if (texture.nrChannels == 4) {
                    internalFormat = GL_RGBA8;
                    sourceFormat = GL_RGBA;
                    sourceType = GL_UNSIGNED_BYTE;
                }
                else {
                    internalFormat = GL_RGB8;
                    sourceFormat = GL_RGB;
                    sourceType = GL_UNSIGNED_BYTE;
                }
         
            }

            for (int i = 0; i < 6; i++) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, sourceFormat, sourceType, data[i].data());
            }
        }, texture.dataVector);


        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        texture.gpuID = textureID;

        m_loadedTextures[texture.id] = texture;
        return textureID;
    }

    uint64_t ResourceManager::loadSkybox(uint64_t hash)
    {

            Skybox skybox = m_assetManager.loadSkyboxFromDisk(hash);
            loadCubemap(skybox.cubeMapTexture);
            loadCubemap(skybox.irradienceMapTexture);

            m_loadedSkyboxes[skybox.id] = skybox;
            return skybox.id;
    }

    uint64_t ResourceManager::loadMesh(uint64_t hash)
    {
        m_loadedMeshes[hash] = m_assetManager.loadMeshFromDisk(hash);
        return hash; // do we really need this lol?
    }

    unsigned int ResourceManager::createMaterial(Material* material)
    {
        return 0;
    }

    // check if material is loaded, if it is loaded return it's id, else return -1
    unsigned int ResourceManager::materialLoaded(unsigned int id)
    {

        auto it = m_loadedmaterials.find(id);
        if (it != m_loadedmaterials.end()) {
            return it->first;
        }

        return -1;
    }

    Material& ResourceManager::getMaterial(uint32_t id)
    {
        return m_loadedmaterials[id];
    }
    

    // Check if texture is loaded, if it is loaded return it's id, else return -1
    uint64_t ResourceManager::textureLoaded(uint64_t hash)
    {
        auto it = m_loadedTextures.find(hash);
        if (it != m_loadedTextures.end()){
            return it->second.id;
        }
        return -1;
    }

    // return the index of the loaded mesh or -1 if its not loaded
    bool ResourceManager::meshLoaded(uint64_t hash)
    {
        auto it = m_loadedMeshes.find(hash);
        if (it != m_loadedMeshes.end()) {
            Log::info("Mesh already loaded");
            return true;
        }

        return false;
    }

    bool ResourceManager::modelLoaded(uint64_t hash) {

        auto it = m_loadedModels.find(hash);
        if (it == m_loadedModels.end()) {
            return false;
        }
        return true;
    }

    void ResourceManager::clearResources()
	{
        Log::info("Clearing resources");


        // list of textures to delete
        std::vector<GLuint> texturesToDelete;

        // because we cant get a clean internal datastructure of an unordered map
        for (auto& texture : m_loadedTextures) {
            texturesToDelete.push_back(texture.second.id);
        }
        
        if (!texturesToDelete.empty()) {
            // by using the vector we save gl calls.
            glDeleteTextures(texturesToDelete.size(), texturesToDelete.data());
        }
        // finally clear the old cpu data
		m_loadedTextures.clear();


        for (auto& mesh : m_loadedMeshes) {
            glDeleteBuffers(1, &mesh.second.VBO);
            glDeleteBuffers(1, &mesh.second.EBO);
            glDeleteVertexArrays(1, &mesh.second.VAO);
        }
		m_loadedMeshes.clear();

		m_loadedmaterials.clear();
		m_loadedModels.clear();
		m_nextMeshID = 0;
		m_nextMaterialID = 0;
    }





}


