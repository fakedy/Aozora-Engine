#pragma once


namespace Aozora {

	class FrameBuffer {
	public:



		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual unsigned int create() = 0;

		virtual void bufferTexture() = 0;

		virtual void createTextures(int width, int height) = 0;
		unsigned int m_colorTextureID{};
		unsigned int m_depthTextureID{};

	private:

	protected:
	};

}