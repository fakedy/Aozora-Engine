#include "OpenglFrameBuffer.h"
#include <glad/glad.h>

Aozora::OpenglFrameBuffer::OpenglFrameBuffer(FrameBufferSpecification spec)
{

	glGenFramebuffers(1, &framebufferID);

	frameBufferSpec = spec;


	for (const auto& attachment : frameBufferSpec.attachments) {

		if (attachment.textureFormat == TextureFormat::DEPTH24STENCIL8) {
			glGenTextures(1, &m_depthTextureID);
			glBindTexture(GL_TEXTURE_2D, m_depthTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, frameBufferSpec.width,
				frameBufferSpec.height, 0, toOpenGLDataFormat(attachment.dataFormat), toOpenGLDataType(attachment.dataType), nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGLFilter(attachment.textureFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGLFilter(attachment.textureFilter));
		}
		else {

			uint32_t colorTextureID;
			glGenTextures(1, &colorTextureID);
			glBindTexture(GL_TEXTURE_2D, colorTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, toOpenGLInternalFormat(attachment.textureFormat), frameBufferSpec.width,
				frameBufferSpec.height, 0, toOpenGLDataFormat(attachment.dataFormat), toOpenGLDataType(attachment.dataType), nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGLFilter(attachment.textureFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGLFilter(attachment.textureFilter));

			m_colorAttachments.push_back(colorTextureID);
		}

	}


}

void Aozora::OpenglFrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	
}

void Aozora::OpenglFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t Aozora::OpenglFrameBuffer::getFrameBufferID()
{
	return framebufferID;
}

void Aozora::OpenglFrameBuffer::buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

	std::vector<GLenum> drawBuffers;
	int index = 0;
	for (uint32_t attachmentID : m_colorAttachments) {

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, attachmentID, 0);
		drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
		index++;
	}
	if (!drawBuffers.empty()) {
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());
	}

	if (m_depthTextureID != 0) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthTextureID, 0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Aozora::OpenglFrameBuffer::updateTexture(unsigned int width, unsigned int height)
{

	frameBufferSpec.width = width;
	frameBufferSpec.height = height;

	int index = 0;
	for (const auto& attachment : frameBufferSpec.attachments) {

		if (attachment.textureFormat == TextureFormat::DEPTH24STENCIL8) {
			glBindTexture(GL_TEXTURE_2D, m_depthTextureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, frameBufferSpec.width,
				frameBufferSpec.height, 0, toOpenGLDataFormat(attachment.dataFormat), toOpenGLDataType(attachment.dataType), nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGLFilter(attachment.textureFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGLFilter(attachment.textureFilter));
		}
		else {


			glBindTexture(GL_TEXTURE_2D, m_colorAttachments[index]);
			glTexImage2D(GL_TEXTURE_2D, 0, toOpenGLInternalFormat(attachment.textureFormat), frameBufferSpec.width,
				frameBufferSpec.height, 0, toOpenGLDataFormat(attachment.dataFormat), toOpenGLDataType(attachment.dataType), nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGLFilter(attachment.textureFilter));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGLFilter(attachment.textureFilter));

		}
		index++;

	}
}

uint32_t Aozora::OpenglFrameBuffer::toOpenGLFilter(TextureFilter filter)
{
	switch (filter)
	{
	case Aozora::FrameBuffer::TextureFilter::Linear:
		return GL_LINEAR;
	case Aozora::FrameBuffer::TextureFilter::Nearest:
		return GL_NEAREST;
	}
	return 0;
}

uint32_t Aozora::OpenglFrameBuffer::toOpenGLInternalFormat(TextureFormat internalFormat)
{
	switch (internalFormat)
	{
	case Aozora::FrameBuffer::TextureFormat::RGBA8:
		return GL_RGBA8;
	case Aozora::FrameBuffer::TextureFormat::RGB8:
		return GL_RGB8;
	case Aozora::FrameBuffer::TextureFormat::RGBA16F:
		return GL_RGBA16F;
	case Aozora::FrameBuffer::TextureFormat::RGBA32F:
		return GL_RGBA32F;
	case Aozora::FrameBuffer::TextureFormat::R32_INT:
		return GL_R32I;
	case Aozora::FrameBuffer::TextureFormat::DEPTH24STENCIL8:
		return GL_DEPTH24_STENCIL8;
	case Aozora::FrameBuffer::TextureFormat::DEPTH32F:
		return GL_DEPTH_COMPONENT32F;
	case Aozora::FrameBuffer::TextureFormat::R8:
		return GL_R8;
	}
	return 0;
}

uint32_t Aozora::OpenglFrameBuffer::toOpenGLWrap(TextureWrap wrap)
{
	switch (wrap)
	{
	case Aozora::FrameBuffer::TextureWrap::ClampToEdge:
		return GL_CLAMP_TO_EDGE;
	case Aozora::FrameBuffer::TextureWrap::Repeat:
		return GL_REPEAT;
	case Aozora::FrameBuffer::TextureWrap::ClampToBorder:
		return GL_CLAMP_TO_BORDER;
	}
	return 0;
}

uint32_t Aozora::OpenglFrameBuffer::toOpenGLDataFormat(DataFormat format)
{
	switch (format)
	{
	case DataFormat::RGB:           
		return GL_RGB;
	case DataFormat::RGBA:         
		return GL_RGBA;
	case DataFormat::RED_INTEGER:   
		return GL_RED_INTEGER;
	case DataFormat::DEPTH_STENCIL: 
		return GL_DEPTH_STENCIL;
	}
	return 0;
}

uint32_t Aozora::OpenglFrameBuffer::toOpenGLDataType(DataType type)
{
	switch (type)
	{
	case DataType::UNSIGNED_BYTE:       
		return GL_UNSIGNED_BYTE;
	case DataType::FLOAT:               
		return GL_FLOAT;
	case DataType::UNSIGNED_INT_24_8:   
		return GL_UNSIGNED_INT_24_8;
	}
	return 0;
}
