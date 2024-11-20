#include "OpenGL.h"
#include <iostream>
#include "Opengl/OpenglShader.h"
#include <GLFW/glfw3.h>
#include "Systems/Windows/Window.h"

namespace Aozora {

	Renderer* Renderer::create(Window::WindowProps props) {
		return new OpenGL(props);
	}

	OpenGL::OpenGL(Window::WindowProps props)
	{
		m_props = props;
		init();
	}

	void OpenGL::init() {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
	}


	void OpenGL::render(Shader& shader, glm::mat4 model, glm::mat4 view, glm::mat4 proj)
	{

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "proj"), 1, GL_FALSE, &proj[0][0]);

	}

	void OpenGL::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	}

	void OpenGL::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

}