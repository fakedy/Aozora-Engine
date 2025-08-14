#pragma once
#include <stdint.h>
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/variant.hpp>
#include <variant>
namespace Aozora {

	class Texture {
	public:

		enum TextureType {
			Texture2D,
			Cubemap
		};

		uint32_t gpuID{};
		uint64_t id{};
		std::string name{};
		uint64_t handle{}; // for bindless textures
		uint32_t refCount{};
		TextureType type{};
		std::string path{};
		uint32_t width, height, nrChannels;
		
		using PixelData = std::variant<std::vector<std::vector<uint8_t>>, std::vector<std::vector<float>>>;
		// each element is the data of a texture
		PixelData dataVector;
		bool hasData{ false };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(id),
				CEREAL_NVP(refCount),
				CEREAL_NVP(type),
				CEREAL_NVP(path),
				CEREAL_NVP(width),
				CEREAL_NVP(height),
				CEREAL_NVP(nrChannels),
				CEREAL_NVP(dataVector),
				CEREAL_NVP(name),
				CEREAL_NVP(hasData));
		}

	private:

	};
}
