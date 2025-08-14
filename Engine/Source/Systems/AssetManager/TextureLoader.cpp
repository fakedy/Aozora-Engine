#include "TextureLoader.h"
#include <iostream>
#include <fstream>
#include <Application.h>
#include <Systems/Logging/Logger.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <xxHash/xxhash.h>

namespace Aozora::Resources {


 
	Texture TextureLoader::loadTexture(const std::string& fileName, const std::string& directory)
	{
        // this is without the parent directory which we need to create unique hashes
        std::string filename = std::string(fileName);
        std::string fullPath = directory + "/" + filename;
  
        Log::info("Loading texture file: " + fullPath);
        if (m_importRegistry.count(fullPath)) {
            Log::info(std::format("{} already exist on disk", filename));
            return Texture();
        }


        int width, height, nrChannels;

        Texture texture;

        if (stbi_is_hdr(fullPath.c_str())) {
            float* data = stbi_loadf(fullPath.c_str(), &width, &height, &nrChannels, 0);
            if (!data) {
                Log::error("Failed to load texture: " + fullPath);
                stbi_image_free(data);
                return Texture();
            }
            std::vector<std::vector<float>> dataV(1);
            dataV[0].resize(width * height * nrChannels);
            memcpy(dataV[0].data(), data, width * height * nrChannels * sizeof(float));
            texture.dataVector = dataV;
            texture.hasData = true;
        }
        else {
            unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

            if (!data) {
                Log::error("Failed to load texture: " + fullPath);
                stbi_image_free(data);
                return Texture();
            }

            std::vector<std::vector<uint8_t>> dataV(1);
            dataV[0].resize(width * height * nrChannels);
            memcpy(dataV[0].data(), data, width * height * nrChannels);
            texture.dataVector = dataV;
            texture.hasData = true;
        }

        texture.width = width;
        texture.height = height;
        texture.nrChannels = nrChannels;
        texture.name = filename;


        m_importRegistry[fullPath] = XXH64(fullPath.c_str(), fullPath.length(), 0);
        texture.id = m_importRegistry[fullPath];
        Log::info("Successfully loaded texture: " + fullPath);
		return texture;
	}

    // just copy of above without directory
    // can prob fix it so i only have to use one later.
    Texture TextureLoader::loadTexture(const std::string& fileName)
    {
        // this is without the parent directory which we need to create unique hashes
        std::string filename = std::string(fileName);

        Log::info("Loading texture file: " + fileName);
        if (m_importRegistry.count(fileName)) {
            Log::info(std::format("{} already exist on disk", filename));
            return Texture();
        }


        int width, height, nrChannels;

        Texture texture;

        if (stbi_is_hdr(fileName.c_str())) {
            float* data = stbi_loadf(fileName.c_str(), &width, &height, &nrChannels, 0);
            if (!data) {
                Log::error("Failed to load texture: " + fileName);
                stbi_image_free(data);
                return Texture();
            }
            std::vector<std::vector<float>> dataV(1);
            dataV[0].resize(width * height * nrChannels);
            memcpy(dataV[0].data(), data, width * height * nrChannels * sizeof(float));
            texture.dataVector = dataV;
            texture.hasData = true;
        }
        else {
            unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);

            if (!data) {
                Log::error("Failed to load texture: " + fileName);
                stbi_image_free(data);
                return Texture();
            }

            std::vector<std::vector<uint8_t>> dataV(1);
            dataV[0].resize(width * height * nrChannels);
            memcpy(dataV[0].data(), data, width * height * nrChannels);
            texture.dataVector = dataV;
            texture.hasData = true;
        }

        texture.width = width;
        texture.height = height;
        texture.nrChannels = nrChannels;
        texture.name = filename;


        m_importRegistry[fileName] = XXH64(fileName.c_str(), fileName.length(), 0);
        texture.id = m_importRegistry[fileName];
        Log::info("Successfully loaded texture: " + fileName);
        return texture;
    }


    Texture TextureLoader::loadCubemap(const std::vector<std::string>& faces)
    {

        // should handle non hdr too but im so lazy atm

        /*
        if (m_importRegistry.count(faces[0])) {
            Log::info(std::format("{} already exist on disk", faces[0]));
            return Texture();
        }
        */

        Texture texture;
        texture.type = Texture::TextureType::Cubemap;
        int width, height, nrChannels;

        std::vector<std::vector<float>> dataV(6);
        int i = 0;
        for (const auto& face : faces) {

            std::string filename = face;
            float* data = stbi_loadf(filename.c_str(), &width, &height, &nrChannels, 0);
            texture.width = width;
            texture.height = height;
            texture.nrChannels = nrChannels;
            if (data) {
                dataV[i].resize(width * height * nrChannels);
                memcpy(dataV[i].data(), data, width * height * nrChannels * sizeof(float));
                stbi_image_free(data);
                texture.hasData = true;
            }
            else {
                Log::error("ResourceManager::loadCubemap failed");
                stbi_image_free(data);
            }
            i++;
        }

        texture.dataVector = dataV;
        m_importRegistry[faces[0]] = XXH64(faces[0].c_str(), faces[0].length(), 0);
        texture.id = m_importRegistry[faces[0]];
        Log::info("Successfully loaded cubemap texture");
        return texture;


    }
}
