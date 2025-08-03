#pragma once
#include "IrenderAPI.h"
#include "glad/glad.h"
#include "Opengl/OpenglShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/Windows/Window.h"
#include <unordered_map>
#include "Opengl/OpenglFrameBuffer.h"


namespace Aozora {

	class OpenGL : public IrenderAPI
	{
	public:

		OpenGL();

		void clear(float r, float g, float b, float a) override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual uint32_t bakeCubemapIrradiance(uint32_t sourceID, uint32_t targetID) override;


	private:
		Mesh irradianceBox;
		OpenglShader m_irradianceShader{ OpenglShader("Resources/Shaders/v_irradiance.glsl", "Resources/Shaders/f_irradiance.glsl") };
	
		void init();
	};
}


