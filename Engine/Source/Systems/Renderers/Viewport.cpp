#include "Viewport.h"
#include "Opengl/OpenglFrameBuffer.h"

Aozora::Viewport::Viewport(uint32_t width, uint32_t height) : width(width), height(height)
{
	framebuffer = std::make_unique<OpenglFrameBuffer>();
	framebuffer->create();
	framebuffer->createTextures(width, height);
	framebuffer->bufferTexture();

}
