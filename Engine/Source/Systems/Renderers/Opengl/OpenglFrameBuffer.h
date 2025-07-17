#pragma once
#include "Systems/Renderers/FrameBuffer.h"
#include <cstdint>

namespace Aozora {

	class OpenglFrameBuffer : public FrameBuffer{
	public:

		OpenglFrameBuffer();
		
		virtual void bind() override;
		virtual void unbind() override;

		virtual unsigned int create() override;

		virtual void bufferTexture() override;
		virtual void createTextures(unsigned int width, unsigned int height) override;

		virtual void updateTexture(unsigned int width, unsigned int height) override;

		uint32_t framebufferID;


	private:


	};




}