#include "ResourceManager.h"
#include <Systems/Logging/Logger.h>

namespace Aozora {


    const void ResourceManager::loadModel(uint64_t hash)
    {
       
       const Model& model = m_assetManager.loadModel(hash);

       for (const auto& node : model.allNodes) {
           if (node.hasMesh) {
               Mesh mesh = m_assetManager.loadMesh(node.meshID);
               m_loadedMeshes[node.meshID] = mesh;
               m_loadedmaterials[mesh.materialID] = m_assetManager.loadMaterial(mesh.materialID);

               for (uint64_t texID : m_loadedmaterials[mesh.materialID].textureIDs) {
                   loadTexture(texID);
               }
           }
       }

       m_loadedModels.emplace(hash, model);
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

        Texture tex = m_assetManager.loadTexture(hash);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        GLenum internalFormat;
        GLenum dataFormat;

        if (tex.nrChannels == 4) {
            internalFormat = tex.isSrgb ? GL_SRGB_ALPHA : GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else {
            internalFormat = tex.isSrgb ? GL_SRGB : GL_RGB8;
            dataFormat = GL_RGB;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tex.width, tex.height, 0, dataFormat, GL_UNSIGNED_BYTE, tex.data.data());
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        // filename will be unique so its alright to use the filename
        // TEMPORARY
        /*
        if (persistent) {
            m_loadedPersistentTextures[filename].id = texture;
            m_loadedPersistentTextures[filename].refCount++;
        }
        else {
            m_loadedTextures[hash].id = texture;
            m_loadedTextures[hash].refCount++;
        }
        */

        uint64_t handle = glGetTextureHandleARB(texture);
        glMakeTextureHandleResidentARB(handle);
        tex.handle = handle;
        m_loadedTextures[hash] = tex;
        Log::info(std::format("Created texture with ID: {}", texture));

        return texture;
        
    }
    // opengl dependent
    unsigned int ResourceManager::loadCubemap(const std::vector<std::string> faces)
    {
        /*

        // TECHNICALLY UNTRACKED TEXTURE



        uint32_t textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels;

        int i = 0;
        for (auto face : faces) {

            std::string filename = face;
            float* data = stbi_loadf(filename.c_str(), &width, &height, &nrChannels, 0);
            if (data) {

                if (nrChannels == 4) {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, data);
                }
                else {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, data);
                }

                stbi_image_free(data);

            }
            else {
                Log::error("ResourceManager::loadCubemap failed");
                stbi_image_free(data);
                return 0;
            }
            i++;
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Log::info(std::format("Created cubemap with ID: {}", textureID));
        return textureID;
        */
        return 0;
    }

    unsigned int ResourceManager::loadCubemap()
    {
        /*
        // TECHNICALLY UNTRACKED TEXTURE

        uint32_t textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        // can put this in parameters instead but lazy
        int width = 32;
        int height = 32;

        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA16F, 32, 32, 0, GL_RGBA, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Log::info(std::format("Created empty cubemap with ID: {}", textureID));

        return textureID;
        */
        return 0;
    }

    uint64_t ResourceManager::loadMesh(uint64_t hash)
    {
        m_loadedMeshes[hash] = m_assetManager.loadMesh(hash);
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


