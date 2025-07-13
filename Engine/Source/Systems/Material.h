#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Aozora {

	class Material {

		// shader
	public:

		struct Texture {
			unsigned int id{};
			unsigned int refCount{};
			std::string type{};
			std::string path{};
		};

		std::vector<Texture> activeTextures;

		Texture diffuseTexture;
		Texture normalTexture;
		Texture heightTexture;
		Texture emissiveTexture;
		Texture aoTexture;
		Texture metallicTexture;
		Texture roughnessTexture;

		glm::fvec3 baseColor{ glm::fvec3(0.3f,1.0f,1.0f) };
		float metallic{ 0.0f };
		float specular{ 0.0f };
		float roughness{ 0.0f };
		float ao{ 0.0f };
		glm::fvec3 emissive{ glm::fvec3(0.0f,0.0f,0.0f) };

	private:
		unsigned int ShaderProgram;

	};


}