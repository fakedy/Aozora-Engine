#include "OpenglFrameBuffer.h"
#include <glad/glad.h>

Aozora::OpenglFrameBuffer::OpenglFrameBuffer()
{
}

void Aozora::OpenglFrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
}

void Aozora::OpenglFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Aozora::OpenglFrameBuffer::create()
{

	glGenFramebuffers(1, &framebuffer);
	return framebuffer;
}

void Aozora::OpenglFrameBuffer::bufferTexture(unsigned int colorTextureID, unsigned int depthTextureID)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);

	// depth/stencil attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
