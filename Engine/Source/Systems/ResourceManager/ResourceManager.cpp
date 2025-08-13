#include "ResourceManager.h"
#include <Systems/Logging/Logger.h>

namespace Aozora {


    const void ResourceManager::loadModel(uint64_t hash)
    {
        // cause crash if it doesnt load
       // m_assetManager.loadAsset(hash);

        

        //m_loadedModels.emplace(loadedModel.name, std::move(loadedModel));
    }

    // opengl dependent code for loading texture
    unsigned int ResourceManager::loadTexture(const std::string path, bool isSrgb, bool persistent)
    {
        /*
        std::string filename = std::string(path);
        unsigned int texture = 0;

        // Checks if the texture is already in memory if so then reference it by id
        int isAlreadyLoaded = textureLoaded(filename);
        if (isAlreadyLoaded != -1) {

            return isAlreadyLoaded;
        }

        Log::info("Loading texture file: " + filename);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (data) {
            GLenum internalFormat;
            GLenum dataFormat;

            if (nrChannels == 4) {
                internalFormat = isSrgb ? GL_SRGB_ALPHA : GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else {
                internalFormat = isSrgb ? GL_SRGB : GL_RGB8;
                dataFormat = GL_RGB;
            }
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            Log::error("ResourceManager::loadTexture failed");
            return 0;
        }
        stbi_image_free(data);

        // filename will be unique so its alright to use the filename
        if (persistent) {
            m_loadedPersistentTextures[filename].id = texture;
            m_loadedPersistentTextures[filename].refCount++;
        }
        else {
            m_loadedTextures[filename].id = texture;
            m_loadedTextures[filename].refCount++;
        }

        Log::info(std::format("Created texture with ID: {}", texture));

        return texture;
        */
        return 0;
    }
    // opengl dependent code for loading texture
    unsigned int ResourceManager::loadTexture(const std::string fileName, const std::string& directory, bool isSrgb, bool persistent)
    {
        /*
        std::string filename = std::string(fileName);
        filename = directory + "/" + filename;
        unsigned int texture = 0;

        // Checks if the texture is already in memory if so then reference it by id
        int isAlreadyLoaded = textureLoaded(filename);
        if (isAlreadyLoaded != -1) {

            return isAlreadyLoaded;
        }

        Log::info("Loading texture file: " + filename);
        int width, height, nrChannels;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (data) {
            GLenum internalFormat;
            GLenum dataFormat;

            if (nrChannels == 4) {
                internalFormat = isSrgb ? GL_SRGB_ALPHA : GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else {
                internalFormat = isSrgb ? GL_SRGB : GL_RGB8;
                dataFormat = GL_RGB;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            Log::error("ResourceManager::loadTexture failed");
            return 0;
        }
        stbi_image_free(data);

        // filename will be unique so its alright to use the filename
        // filename will be unique so its alright to use the filename
        if (persistent) {
            m_loadedPersistentTextures[filename].id = texture;
            m_loadedPersistentTextures[filename].refCount++;
        }
        else {
            m_loadedTextures[filename].id = texture;
            m_loadedTextures[filename].refCount++;
        }

        Log::info(std::format("Created texture with ID: {}", texture));
        return texture;
        */
        return 0;
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
    int ResourceManager::textureLoaded(const std::string path)
    {
        auto it = m_loadedTextures.find(path);
        if (it != m_loadedTextures.end()){
            return it->second.id;
        }
        return -1;
    }

    // return the index of the loaded mesh or -1 if its not loaded
    unsigned int ResourceManager::meshLoaded(const std::string path)
    {
        auto it = m_meshPathToID.find(path);
        if (it != m_meshPathToID.end()) {
            Log::info("Mesh already loaded");
            return it->second;
        }

        return -1;
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

        // delete vertex buffers etc from gpu
		m_meshPathToID.clear();

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


