#pragma once
#include "Systems/Renderers/FrameBuffer.h"
#include <cstdint>

namespace Aozora {

	class OpenglFrameBuffer : public FrameBuffer{
	public:

		OpenglFrameBuffer(FrameBufferSpecification spec);
		
		virtual void bind() override;
		virtual void unbind() override;


		virtual void buffer() override;
		virtual uint32_t getFrameBufferID() override;

		virtual void updateTexture(unsigned int width, unsigned int height) override;



	private:

		uint32_t toOpenGLTarget(TextureTarget target);
		uint32_t toOpenGLFilter(TextureFilter filter);
		uint32_t toOpenGLInternalFormat(TextureFormat internalFormat);
		uint32_t toOpenGLWrap(TextureWrap wrap);
		uint32_t toOpenGLDataFormat(DataFormat format);
		uint32_t toOpenGLDataType(DataType type);
	};




}