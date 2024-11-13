#include "OpenGL.h"
#include <iostream>
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/MeshComponent.h"
#include "Opengl/OpenglShader.h"

namespace Aozora {

	Renderer* Renderer::create(std::shared_ptr<entt::registry> registry) {
		return new OpenGL(registry);
	}

	OpenGL::OpenGL(std::shared_ptr<entt::registry> registry)
	{
		m_registry = registry;
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

		auto view = m_registry->view<const meshComponent>(); // register of all mesh components

		for (const auto entity : view) {
			auto& mesh = view.get<meshComponent>(entity);
			glBindVertexArray(mesh.mesh->VAO);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.shaderProgram, "model"), 1, GL_FALSE,  &model[0][0]);
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::translate(view, glm::vec3(-1.0f, -1.0f, -3.0f));
			glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
			glm::mat4 proj = glm::mat4(1.0f);
			proj = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
			glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.shaderProgram, "proj"), 1, GL_FALSE, &proj[0][0]);

			glDrawElements(GL_TRIANGLES, mesh.mesh->indicesSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

	}

}