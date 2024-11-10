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

void Aozora::OpenglFrameBuffer::bufferTexture(unsigned int textureID)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
