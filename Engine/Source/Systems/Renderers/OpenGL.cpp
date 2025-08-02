#include "OpenGL.h"
#include <iostream>
#include "Opengl/OpenglShader.h"
#include <GLFW/glfw3.h>
#include "Systems/Windows/Window.h"

#include "Systems/ECS/Components/Components.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Application.h"


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
	}



	void OpenGL::clear(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}


}