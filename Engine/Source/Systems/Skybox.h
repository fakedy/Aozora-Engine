#pragma once
#include <stdint.h>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>



class Skybox {
public:

	uint64_t id{ 0 };
	uint64_t cubeMapTexture{ 0 };
	uint64_t irradienceMapTexture{ 0 };

	template<class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(id),
			CEREAL_NVP(cubeMapTexture),
			CEREAL_NVP(irradienceMapTexture));
	}
};