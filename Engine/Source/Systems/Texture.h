#pragma once
#include <stdint.h>
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

namespace Aozora {

	class Texture {
	public:

		enum TextureType {
			DIFFUSE,
			NORMAL,
			EMISSIVE,
			AO,
			METALLIC,
			ROUGHNESS
		};


		uint32_t id{};
		std::string name{};
		uint64_t handle{}; // for bindless textures
		uint64_t hash{};
		uint32_t refCount{};
		TextureType type{};
		std::string path{};
		uint32_t width, height, nrChannels;
		std::vector<uint8_t> data;
		bool isSrgb{ false };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(id),
				CEREAL_NVP(refCount),
				CEREAL_NVP(type),
				CEREAL_NVP(path),
				CEREAL_NVP(width),
				CEREAL_NVP(height),
				CEREAL_NVP(nrChannels),
				CEREAL_NVP(data),
				CEREAL_NVP(name),
				CEREAL_NVP(isSrgb));
		}

	private:

	};
}
