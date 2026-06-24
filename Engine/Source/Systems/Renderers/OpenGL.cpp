#include "OpenGL.h"
#include <iostream>
#include "Opengl/OpenglShader.h"
#include "Systems/Renderers/FrameBuffer.h"
#include "Systems/Renderers/Opengl/OpenglFrameBuffer.h"
#include <Systems/Logging/Logger.h>

namespace Aozora {

	IrenderAPI* IrenderAPI::create(){
		return new OpenGL();
	}

	OpenGL::OpenGL()
	{
		init();
	}

	void OpenGL::init() {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glGenVertexArrays(1, &VAO);
	}


	// change this to more modular design
	void OpenGL::clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	// takes in a source and target texture
	uint32_t OpenGL::bakeCubemapIrradiance(uint32_t sourceID, uint32_t targetID, uint32_t width, uint32_t height)
	{
		Log::info("Baking Cubemap Irradiance Texture");
		
		// setup FBO
		uint32_t captureFBO;
		glGenFramebuffers(1, &captureFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glUseProgram(m_irradianceShader.ID);
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, sourceID);

		glViewport(0, 0, width, height);
		// set projection matrix
		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		m_irradianceShader.setInt("environmentMap", 0);
		m_irradianceShader.setMat4("proj", proj);

		// one for each face
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // Positive X
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // Negative X
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)), // Positive Y
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)), // Negative Y
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // Positive Z
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))  // Negative Z
		};

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		// one render for each face of the cube, its why we have a captureViews vector too
		for (uint32_t i = 0; i < 6; i++) {

			// set view matrix
			m_irradianceShader.setMat4("view", captureViews[i]);

			// attach the cubemap face to FBO
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, targetID, 0);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				Log::error("framebuffer not complete");
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFBO);


		return targetID;
	}

	// code repetition but i'll have to clean stuff later
	uint32_t OpenGL::bakeCubemapPrefilter(uint32_t sourceID, uint32_t targetID, uint32_t width, uint32_t height)
	{
		Log::info("Baking Cubemap Prefilter Texture");

		// setup FBO
		uint32_t captureFBO;
		glGenFramebuffers(1, &captureFBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glUseProgram(m_prefilterShader.ID);
		glBindVertexArray(VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, sourceID);

		// set projection matrix
		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		m_prefilterShader.setInt("environmentMap", 0);
		m_prefilterShader.setMat4("proj", proj);

		// one for each face
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // Positive X
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // Negative X
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)), // Positive Y
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)), // Negative Y
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)), // Positive Z
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))  // Negative Z
		};

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		uint32_t mipmapLevels = 5;

		for (uint32_t i = 0; i < mipmapLevels; ++i)
		{
			unsigned int mipWidth = width * std::pow(0.5, i);
			unsigned int mipHeight = height * std::pow(0.5, i);
			glViewport(0, 0, width, height);
			float roughness = (float)i / (float)(mipmapLevels - 1);
			m_prefilterShader.setFloat("roughness", roughness);

			// one render for each face of the cube, its why we have a captureViews vector too
			for (uint32_t i = 0; i < 6; i++) {

				// set view matrix
				m_prefilterShader.setMat4("view", captureViews[i]);

				// attach the cubemap face to FBO
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, targetID, i);

				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					Log::error("framebuffer not complete");
				}

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &captureFBO);


		return targetID;
	}

	Texture OpenGL::loadEmptyCubemap(Texture::TextureSpecification specification)
	{

		Texture tex;
		tex.specification = specification;

		uint32_t target = toOpenGLTarget(specification.config.target);

		uint32_t textureID;

		glGenTextures(1, &textureID);
		glBindTexture(target, textureID);


		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, toOpenGLInternalFormat(specification.config.internalFormat),
				specification.width,
				specification.height, 0, toOpenGLDataFormat(specification.config.dataFormat),
				toOpenGLDataType(specification.config.dataType), nullptr);
		}


		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, toOpenGLFilter(specification.config.minFilter));
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, toOpenGLFilter(specification.config.magFilter));

		if (specification.mipmaps) {
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}
		// no support for different axises
		glTexParameteri(target, GL_TEXTURE_WRAP_S, toOpenGLWrap(specification.config.wrap));
		glTexParameteri(target, GL_TEXTURE_WRAP_T, toOpenGLWrap(specification.config.wrap));
		glTexParameteri(target, GL_TEXTURE_WRAP_R, toOpenGLWrap(specification.config.wrap));

		tex.gpuID = textureID;

		return tex;
	}


	uint32_t OpenGL::toOpenGLTarget(Texture::TextureTarget target)
	{
		switch (target)
		{
		case Texture::TextureTarget::TEXTURE_2D:
			return GL_TEXTURE_2D;
			break;
		case Texture::TextureTarget::TEXTURE_CUBE_MAP:
			return GL_TEXTURE_CUBE_MAP;
			break;
		default:
			break;
		}
	}


	uint32_t OpenGL::toOpenGLFilter(Texture::TextureFilter filter)
	{
		switch (filter)
		{
		case Texture::TextureFilter::LINEAR:
			return GL_LINEAR;
		case Texture::TextureFilter::NEAREST:
			return GL_NEAREST;
		case Texture::TextureFilter::LINEAR_MIPMAP_LINEAR:
			return GL_LINEAR_MIPMAP_LINEAR;
		case Texture::TextureFilter::LINEAR_MIPMAP_NEAREST:
			return GL_LINEAR_MIPMAP_NEAREST;
		}
		return 0;
	}

	uint32_t OpenGL::toOpenGLInternalFormat(Texture::TextureFormat internalFormat)
	{
		switch (internalFormat)
		{
		case Texture::TextureFormat::RGBA8:
			return GL_RGBA8;
		case Texture::TextureFormat::RGB8:
			return GL_RGB8;
		case Texture::TextureFormat::RGB16F:
			return GL_RGB16F;
		case Texture::TextureFormat::RGBA16F:
			return GL_RGBA16F;
		case Texture::TextureFormat::RGBA32F:
			return GL_RGBA32F;
		case Texture::TextureFormat::R32_INT:
			return GL_R32I;
		case Texture::TextureFormat::DEPTH24STENCIL8:
			return GL_DEPTH24_STENCIL8;
		case Texture::TextureFormat::DEPTH32F:
			return GL_DEPTH_COMPONENT32F;
		case Texture::TextureFormat::R8:
			return GL_R8;
		}
		return 0;
	}

	uint32_t OpenGL::toOpenGLWrap(Texture::TextureWrap wrap)
	{
		switch (wrap)
		{
		case Texture::TextureWrap::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		case Texture::TextureWrap::REPEAT:
			return GL_REPEAT;
		case Texture::TextureWrap::CLAMP_TO_BORDER:
			return GL_CLAMP_TO_BORDER;
		}
		return 0;
	}

	uint32_t OpenGL::toOpenGLDataFormat(Texture::DataFormat format)
	{
		switch (format)
		{
		case Texture::DataFormat::RGB:
			return GL_RGB;
		case Texture::DataFormat::RGBA:
			return GL_RGBA;
		case Texture::DataFormat::RED_INTEGER:
			return GL_RED_INTEGER;
		case Texture::DataFormat::DEPTH_STENCIL:
			return GL_DEPTH_STENCIL;
		}
		return 0;
	}

	uint32_t OpenGL::toOpenGLDataType(Texture::DataType type)
	{
		switch (type)
		{
		case Texture::DataType::UNSIGNED_BYTE:
			return GL_UNSIGNED_BYTE;
		case Texture::DataType::FLOAT:
			return GL_FLOAT;
		case Texture::DataType::UNSIGNED_INT_24_8:
			return GL_UNSIGNED_INT_24_8;
		}
		return 0;
	}


}