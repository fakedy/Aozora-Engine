#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace Aozora {


    const std::vector<Mesh> ResourceManager::loadModel(const std::string& file)
    {

        // I think a system that will check if the model is already loaded is good here.
        // it will ensure we dont load multiple of the same models into memory

        // TODO store model into memory

        return m_modelLoader.loadModel(file);

    }

    unsigned int ResourceManager::loadTexture(const std::string path, const std::string& directory)
    {
        std::string filename = std::string(path);
        filename = directory + "/" + filename;
        unsigned int texture = 0;

        // Checks if the texture is already in memory if so then reference it by id
        int isAlreadyLoaded = textureLoaded(filename);
        if (isAlreadyLoaded != -1) {

            return isAlreadyLoaded;
        }

        int width, height, nrChannels;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
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

        // temp?
        m_loadedTextures[filename].id = texture;
        m_loadedTextures[filename].refCount++;

        return texture;

    }

    int ResourceManager::textureLoaded(const std::string path)
    {
        auto it = m_loadedTextures.find(path);
        if (it != m_loadedTextures.end()){
            return it->second.id;
        }
        return -1;
    }



}


