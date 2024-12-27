#pragma once
#include "Systems/Renderers/FrameBuffer.h"

namespace Aozora {

	class OpenglFrameBuffer : public FrameBuffer{
	public:

		OpenglFrameBuffer();
		
		virtual void bind() override;
		virtual void unbind() override;

		virtual unsigned int create() override;

		virtual void bufferTexture() override;
		virtual void createTextures(int width, int height) override;

	private:

		unsigned int framebuffer;

	};




}