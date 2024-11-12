#include "OpenGL.h"
#include <iostream>
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/MeshComponent.h"


namespace Aozora {


	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aNormal;\n"
		"out vec3 normalCoord;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"	normalCoord = aNormal;\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 normalCoord;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"   FragColor = vec4(normalCoord.x, normalCoord.y, normalCoord.z, 1.0f);\n"
		"}\n\0";


	Renderer* Renderer::create(std::shared_ptr<entt::registry> registry) {
		return new OpenGL(registry);
	}




	OpenGL::OpenGL(std::shared_ptr<entt::registry> registry)
	{
		m_registry = registry;
		init();
	}

	void OpenGL::init() {

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		

	}


	void OpenGL::render()
	{


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		auto view = m_registry->view<const meshComponent>(); // register of all mesh components

		for (const auto entity : view) {
			auto& mesh = view.get<meshComponent>(entity);
			glBindVertexArray(mesh.mesh->VAO);
			glDrawElements(GL_TRIANGLES, mesh.mesh->indicesSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}


	}

}