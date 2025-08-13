#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Texture.h"

#include <Systems/Serialization/SerializationGLM.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>


namespace Aozora {

	class Material {
	public:

		// storing textures like this is not efficient and its expensive
		// the Texture files contain the raw data for that texture
		Texture diffuseTexture;
		Texture normalTexture;
		Texture heightTexture;
		Texture emissiveTexture;
		Texture aoTexture;
		Texture metallicTexture;
		Texture roughnessTexture;
		
		
		// Default values incase we render without texture
		glm::fvec4 baseColor{ glm::fvec4(0.3f, 1.0f, 1.0f , 1.0f) };
		float metallic{ 0.0f };
		float specular{ 0.0f };
		float roughness{ 0.0f };
		float ao{ 1.0f };
		glm::fvec4 emissive{ glm::fvec4(0.0f,0.0f,0.0f, 0.0f) };

		uint32_t ID;
		uint64_t hash{};
		std::string name;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(diffuseTexture),
				CEREAL_NVP(normalTexture),
				CEREAL_NVP(heightTexture),
				CEREAL_NVP(emissiveTexture),
				CEREAL_NVP(aoTexture),
				CEREAL_NVP(metallicTexture),
				CEREAL_NVP(roughnessTexture),
				CEREAL_NVP(baseColor),
				CEREAL_NVP(metallic),
				CEREAL_NVP(specular),
				CEREAL_NVP(roughness),
				CEREAL_NVP(ao),
				CEREAL_NVP(emissive),
				CEREAL_NVP(ID),
				CEREAL_NVP(name));
		}
	};

}