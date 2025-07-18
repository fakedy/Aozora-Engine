#pragma once
#include <vector>

namespace Aozora {

	class FrameBuffer {
	public:

		enum class TextureFormat {
			RGBA8,
			RGB8,
			RGBA16F,
			RGBA32F,
			R32_INT,
			DEPTH24STENCIL8,
			DEPTH32F,
			R8
		};

		enum class TextureWrap {
			ClampToEdge,
			Repeat,
			ClampToBorder
		};

		enum class TextureFilter {
			Linear,
			Nearest
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

		struct FrameBufferAttachment {
			TextureFormat textureFormat;
			TextureWrap textureWrap;
			TextureFilter textureFilter;
			DataFormat dataFormat;
			DataType dataType;
		};

		struct FrameBufferSpecification {

			uint32_t width{ 1920 };
			uint32_t height{ 1080 };

			std::vector<FrameBufferAttachment> attachments;

		};


		virtual void bind() = 0;
		virtual void unbind() = 0;


		virtual void buffer() = 0;

		virtual uint32_t getFrameBufferID() = 0;

		virtual void updateTexture(unsigned int width, unsigned int height) = 0;

		unsigned int m_depthTextureID{};

		FrameBufferSpecification frameBufferSpec;


		uint32_t framebufferID{};

		std::vector<uint32_t> m_colorAttachments;



	private:

	protected:
	};

}