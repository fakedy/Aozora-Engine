#pragma once
#include <stdint.h>
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/variant.hpp>
#include <variant>
#include <cereal/types/string.hpp>

namespace Aozora {


	class Texture {
	public:

		enum class TextureTarget {
			TEXTURE_2D,
			TEXTURE_CUBE_MAP
		};

		// internal format
		enum class TextureFormat {
			RGBA8,
			RGB8,
			RGB16F,
			RGBA16F,
			RGBA32F,
			R32_INT,
			DEPTH24STENCIL8,
			DEPTH32F,
			R8
		};

		enum class TextureWrap {
			CLAMP_TO_EDGE,
			REPEAT,
			CLAMP_TO_BORDER
		};

		enum class TextureFilter {
			LINEAR,
			NEAREST,
			LINEAR_MIPMAP_LINEAR,
			LINEAR_MIPMAP_NEAREST
		};

		enum class DataFormat {
			RGB,
			RGBA,
			RED_INTEGER,
			DEPTH_STENCIL
		};

		enum class DataType {
			UNSIGNED_BYTE,
			FLOAT,
			UNSIGNED_INT_24_8
		};

		struct TextureConfig {
			TextureTarget target{ TextureTarget::TEXTURE_2D };
			TextureFormat internalFormat{ TextureFormat::RGBA8 };
			TextureWrap wrap{ TextureWrap::REPEAT };
			TextureFilter minFilter{ TextureFilter::LINEAR };
			TextureFilter magFilter{ TextureFilter::LINEAR };
			DataFormat dataFormat{ DataFormat::RGBA };
			DataType dataType{ DataType::UNSIGNED_BYTE };

			template<class Archive>
			void serialize(Archive& archive) {
				archive(CEREAL_NVP(target),
					CEREAL_NVP(internalFormat),
					CEREAL_NVP(wrap),
					CEREAL_NVP(minFilter),
					CEREAL_NVP(magFilter),
					CEREAL_NVP(dataFormat),
					CEREAL_NVP(dataType));
			}
		};

		struct TextureSpecification {
			uint32_t width{ 1920 };
			uint32_t height{ 1080 };
			TextureConfig config;
			bool mipmaps{ false };
			float maxAnisotropy{ 1.0f };

			template<class Archive>
			void serialize(Archive& archive) {
				archive(CEREAL_NVP(width),
					CEREAL_NVP(height),
					CEREAL_NVP(config));
			}
		};


		uint32_t gpuID{};
		uint64_t hash{};
		std::string name{};
		uint64_t handle{}; // for bindless textures
		uint32_t refCount{};
		std::string path{};
		uint32_t width, height, nrChannels; // redundant
		TextureSpecification specification;
		bool isSrgb{ false };
		using PixelData = std::variant<std::vector<std::vector<uint8_t>>, std::vector<std::vector<float>>>;
		// each element is the data of a texture
		PixelData dataVector;
		bool hasData{ false };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(hash),
				CEREAL_NVP(refCount),
				CEREAL_NVP(specification),
				CEREAL_NVP(path),
				CEREAL_NVP(width),
				CEREAL_NVP(height),
				CEREAL_NVP(nrChannels),
				CEREAL_NVP(dataVector),
				CEREAL_NVP(name),
				CEREAL_NVP(hasData),
				CEREAL_NVP(isSrgb));
		}

	private:

	};
}
