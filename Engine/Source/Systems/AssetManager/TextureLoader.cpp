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
            Log::info(std::format("{} already cached", filename));
            return Texture();
        }


        int width, height, nrChannels;
        unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

        Texture texture;
        texture.width = width;
        texture.height = height;
        texture.nrChannels = nrChannels;
        texture.name = filename;
        if (!data) {
            Log::error("Failed to load texture: " + fullPath);
            stbi_image_free(data);
            return Texture();
        }

        texture.data.resize(width * height * nrChannels);
        memcpy(texture.data.data(), data, width * height * nrChannels);
        stbi_image_free(data);

        m_importRegistry[fullPath] = XXH64(fullPath.c_str(), fullPath.length(), 0);
        texture.id = m_importRegistry[fullPath];
        Log::info("Successfully loaded texture: " + fullPath);
		return texture;
	}

}
