#include "OpenGL.h"
#include <iostream>
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/MeshComponent.h"
#include "Opengl/OpenglShader.h"
#include <GLFW/glfw3.h>
#include "Systems/Windows/Window.h"

namespace Aozora {

	Renderer* Renderer::create(std::shared_ptr<Scene> registry, Window::WindowProps props) {
		return new OpenGL(registry, props);
	}

	OpenGL::OpenGL(std::shared_ptr<Scene> scene, Window::WindowProps props)
	{
		m_scene = scene;
		m_props = props;
		m_registry = scene.get()->registry;
		init();
	}

	void OpenGL::init() {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}


	void OpenGL::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		auto view = m_registry->view<const meshComponent, Transform>(); // register of all mesh components

		for (const auto entity : view) {
			auto& mesh = view.get<meshComponent>(entity);
			auto& transform = view.get<Transform>(entity);

			glBindVertexArray(mesh.mesh->VAO);

			glm::mat4 model(1.0f);
			model = transform.model;
			glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.shaderProgram, "model"), 1, GL_FALSE,  &model[0][0]);
			glm::mat4 view = glm::mat4(1.0f);
			glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
			glm::mat4 proj = glm::mat4(1.0f);
			proj = glm::perspective(glm::radians(45.0f), (float)m_props.width / (float)m_props.height, 0.1f, 100.0f);
			glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.shaderProgram, "proj"), 1, GL_FALSE, &proj[0][0]);

			glDrawElements(GL_TRIANGLES, mesh.mesh->indicesSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	}

	void OpenGL::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

}