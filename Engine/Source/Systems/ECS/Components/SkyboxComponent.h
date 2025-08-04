#pragma once
#include <cereal/cereal.hpp>

namespace Aozora {

	struct SkyboxTextures {
		uint32_t skyboxTextureID{ 0 };
		uint32_t irradianceTextureID{ 0 };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(skyboxTextureID),
				CEREAL_NVP(irradianceTextureID));
		}
	};

	struct SkyboxComponent {
	public:
		SkyboxTextures data;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(data));
		}
	};
}
