#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <Systems/Texture.h>


namespace Aozora::Resources {


	class TextureLoader {
	public:
		TextureLoader(std::unordered_map<std::string, uint64_t>& importRegistry) : m_importRegistry(importRegistry){};

		Texture loadTexture(const std::string& fileName, const std::string& directory, bool isSrgb);
		Texture loadTexture(const std::string& fileName);
		Texture loadCubemap(const std::vector<std::string>& faces);
		Texture loadIrradienceCubemap(uint32_t textureTargetID);


	private:

		std::unordered_map<std::string, uint64_t>& m_importRegistry;

	};
}