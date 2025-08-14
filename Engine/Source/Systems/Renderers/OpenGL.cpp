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
		// temp ?
		glGenVertexArrays(1, &irradianceBox.VAO);
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
	uint32_t OpenGL::bakeCubemapIrradiance(uint32_t sourceID, uint32_t targetID)
	{
		Log::info("Baking Cubemap Irradiance Texture");
		
		// setup FBO
		uint32_t captureFBO;
		glGenFramebuffers(1, &captureFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

		glUseProgram(m_irradianceShader.ID);
		glBindVertexArray(irradianceBox.VAO);

		m_irradianceShader.setInt("environmentMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, sourceID);

		glViewport(0, 0, 32, 32);
		// set projection matrix
		glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
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


}