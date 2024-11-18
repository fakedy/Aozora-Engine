#pragma once


namespace Aozora {

	class FrameBuffer {
	public:



		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual unsigned int create() = 0;

		virtual void bufferTexture(unsigned int colorTextureID, unsigned int depthTextureID) = 0;

	private:

	};

}