#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Aozora {

	class Material {

		// shader
	public:

		enum TextureType{
			DIFFUSE,
			NORMAL,
			EMISSIVE,
			AO,
			METALLIC,
			ROUGHNESS
		};


		struct Texture {
			unsigned int id{};
			unsigned int refCount{};
			TextureType type{};
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

		glm::fvec4 baseColor{ glm::fvec4(0.3f, 1.0f, 1.0f , 1.0f) };
		float metallic{ 0.0f };
		float specular{ 0.0f };
		float roughness{ 0.0f };
		float ao{ 0.0f };
		glm::fvec4 emissive{ glm::fvec4(0.0f,0.0f,0.0f, 0.0f) };

		uint32_t ID;
		std::string name;

	private:
		unsigned int ShaderProgram;

	};


}