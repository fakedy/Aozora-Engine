#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace Aozora {


    const void ResourceManager::loadModel(const std::string& file)
    {
        // imho its bad that i do this filename thing again.
        // storing shorted filename and whole path for the map is confusing
        std::string filename = file.substr(file.find_last_of('/'), file.find_last_of('.'));
        if (modelLoaded(filename)) {
            std::cout << "model already loaded\n";
            return;
        }
        Model loadedModel = m_modelLoader.loadModel(file);

        m_loadedModels.emplace(loadedModel.name, std::move(loadedModel));

    }

    // opengl dependent code for loading texture
    unsigned int ResourceManager::loadTexture(const std::string path, const std::string& directory, bool isSrgb)
    {
        std::string filename = std::string(path);
        filename = directory + "/" + filename;
        unsigned int texture = 0;

        // Checks if the texture is already in memory if so then reference it by id
        int isAlreadyLoaded = textureLoaded(filename);
        if (isAlreadyLoaded != -1) {

            return isAlreadyLoaded;
        }

        std::cout << "Loading texture file: " << filename << "\n";
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
            std::cerr << "ResourceManager: texture load failed\n";
            return 0;
        }
        stbi_image_free(data);

        // filename will be unique so its alright to use the filename
        m_loadedTextures[filename].id = texture;
        m_loadedTextures[filename].refCount++;

        std::cout << "Created texture with ID: " << texture << "\n";

        return texture;

    }

    // opengl dependent
    unsigned int ResourceManager::loadCubemap(const std::vector<std::string> faces)
    {
        // faces[0] isnt reliable but it should work fine for now as faces path include the directory to it
        int isAlreadyLoaded = textureLoaded(faces[0]);
        if (isAlreadyLoaded != -1) {

            return isAlreadyLoaded;
        }

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
                std::cerr << "ResourceManager: texture load failed\n";
                stbi_image_free(data);
                return 0;
            }
            std::cout << "Created face texture\n";
            i++;
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // cache texture
        Material::Texture targetTexture;
        m_loadedTextures[faces[0]] = targetTexture;
        m_loadedTextures[faces[0]].id = textureID;
        m_loadedTextures[faces[0]].refCount++;
        std::cout << "Created cubemap with ID: " << textureID << "\n";

        return textureID;
    }

    unsigned int ResourceManager::loadCubemap()
    {
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

        std::cout << "Created cubemap with ID: " << textureID << "\n";

        return textureID;
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
            std::cout << "Mesh already loaded\n";
            return it->second;
        }

        return -1;
    }

    bool ResourceManager::modelLoaded(const std::string path) {

        auto it = m_loadedModels.find(path);
        if (it == m_loadedModels.end()) {
            return false;
        }
        return true;
    }

    void ResourceManager::clearResources()
	{
        // THIS DOES NOT CLEAR THE GPU RESOURCES RIGHT NOW
        // GPU MEMORY LEAK
		std::cout << "Clearing resources\n";
		m_loadedTextures.clear();
		m_meshPathToID.clear();
		m_loadedMeshes.clear();
		m_loadedmaterials.clear();
		m_loadedModels.clear();
		m_nextMeshID = 0;
		m_nextMaterialID = 0;
    }



}


