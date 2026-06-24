#pragma once
#include "IrenderAPI.h"
#include "glad/glad.h"
#include "Opengl/OpenglShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Opengl/OpenglFrameBuffer.h"


namespace Aozora {
	
	class OpenGL : public IrenderAPI
	{
	public:

		OpenGL();

		void clear(float r, float g, float b, float a) override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual uint32_t bakeCubemapIrradiance(uint32_t sourceID, uint32_t targetID, uint32_t width, uint32_t height) override;
		virtual uint32_t bakeCubemapPrefilter(uint32_t sourceID, uint32_t targetID, uint32_t width, uint32_t height) override;
		virtual Texture loadEmptyCubemap(Texture::TextureSpecification specification) override;


	private:
		GLuint VAO;
		OpenglShader m_irradianceShader{ OpenglShader("Resources/Shaders/v_irradiance.glsl", "Resources/Shaders/f_irradiance.glsl") };
		OpenglShader m_prefilterShader{ OpenglShader("Resources/Shaders/v_prefilter.glsl", "Resources/Shaders/f_prefilter.glsl") };
	
		void init();

		uint32_t toOpenGLTarget(Texture::TextureTarget target);
		uint32_t toOpenGLFilter(Texture::TextureFilter filter);
		uint32_t toOpenGLInternalFormat(Texture::TextureFormat internalFormat);
		uint32_t toOpenGLWrap(Texture::TextureWrap wrap);
		uint32_t toOpenGLDataFormat(Texture::DataFormat format);
		uint32_t toOpenGLDataType(Texture::DataType type);
	};
}


