#pragma once
#include "Renderer.h"
#include "glad/glad.h"
#include "Opengl/OpenglShader.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Aozora {

	class OpenGL : public Renderer
	{
	public:

		OpenGL(const Window::WindowProps &props);

		void render(Shader& shader, glm::mat4 model, glm::mat4 view, glm::mat4 proj) override;
		void clear() override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;


	private:
	
		void init();
	};
}


