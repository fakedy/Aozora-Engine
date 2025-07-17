#pragma once
#include "IrenderAPI.h"
#include "glad/glad.h"
#include "Opengl/OpenglShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/Windows/Window.h"
#include <unordered_map>
#include "Opengl/OpenglFrameBuffer.h"
#include "Viewport.h"
#include <Systems/Renderers/DeferredPipeline.h>



namespace Aozora {

	class OpenGL : public IrenderAPI
	{
	public:

		OpenGL();

		void clear() override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;



	private:


	
		void init();
	};
}


