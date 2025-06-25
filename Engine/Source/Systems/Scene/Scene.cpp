#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <Application.h>



namespace Aozora {
	Scene::Scene()
	{
		m_registry = std::make_shared<entt::registry>();

	}
	void Scene::update()
	{
		// essentially a system that act on transforms
		auto view = m_registry->view<TransformComponent>(); // register of all transform components
		for (const auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(glm::mat4(1.0f), transform.pos);
			glm::quat rotation = glm::quat(glm::radians(transform.rot));
			model = model * glm::mat4_cast(rotation);
			model = glm::scale(model, transform.scale);
			transform.model = model;
		}

		/*
		


			editorCamera->update();
			renderEditorScene(editorCamera);


		*/
	}



	void Scene::renderEditorScene(std::shared_ptr<EditorCamera> editorCamera)
	{
		/*
		m_renderer->clear();
		glUseProgram(m_defaultShader.ID);
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		auto view = m_registry->view<const MeshComponent, TransformComponent>(); // register of all mesh components
		for (const auto entity : view) {
			auto& meshComponent = view.get<MeshComponent>(entity);
			auto& transform = view.get<TransformComponent>(entity);

			m_renderer->setViewport(0, 0, editorCamera->m_viewPortX, editorCamera->m_viewPortY);
			m_renderer->render(m_defaultShader, transform.model, editorCamera->getView(), editorCamera->getProjection());
			glUniform3fv(glGetUniformLocation(m_defaultShader.ID, "cameraPos"), 1, &editorCamera->getPos()[0]);

			for (unsigned int id : meshComponent.meshIDs) {
				resourceManager.m_loadedMeshes[id].draw(m_defaultShader);
			}
		}


		// render grid

		unsigned int gridVAO;
		glGenVertexArrays(1, &gridVAO);
		glBindVertexArray(gridVAO);
		glUseProgram(m_gridShader.ID);
		glUniform2fv(glGetUniformLocation(m_gridShader.ID, "screenSize"), 1, &glm::vec2(editorCamera->m_viewPortX, editorCamera->m_viewPortY)[0]);
		glUniformMatrix4fv(glGetUniformLocation(m_gridShader.ID, "view"), 1, GL_FALSE, &editorCamera->getView()[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_gridShader.ID, "proj"), 1, GL_FALSE, &editorCamera->getProjection()[0][0]);
		glUniform3fv(glGetUniformLocation(m_gridShader.ID, "cameraPos"), 1, &editorCamera->getPos()[0]);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		*/
	}


	entt::registry& Scene::getRegistry()
	{
		return *m_registry;
	}
}
