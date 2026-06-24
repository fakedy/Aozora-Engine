#pragma once
#include <stdint.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>



class Skybox {
public:

	uint64_t hash{ 0 };
	uint64_t cubeMapHash{ 0 };
	uint64_t irradienceMapHash{ 0 }; // blurred diffuse lighting texture
	uint64_t prefilterMapHash{ 0 }; // specular pre filtered texture

	template<class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(hash),
			CEREAL_NVP(cubeMapHash),
			CEREAL_NVP(irradienceMapHash),
			CEREAL_NVP(prefilterMapHash)
			);
	}
};