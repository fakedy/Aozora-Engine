#include "OpenGL.h"
#include <iostream>
#include "Opengl/OpenglShader.h"
#include <GLFW/glfw3.h>
#include "Systems/Windows/Window.h"

#include "Systems/ECS/Components/Components.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Application.h"


namespace Aozora {

	Renderer* Renderer::create(Window::WindowProps &props){
		return new OpenGL(props);
	}

	void OpenGL::updatePrimaryScene(Scene* scene)
	{

		for (auto& [ID, viewport] : m_viewports) {
		
			if (viewport.type == ViewportType::PrimaryGame || viewport.type == ViewportType::PrimaryEditor) {
				viewport.scene = scene;
			}
		
		}
	}

	OpenGL::OpenGL(Window::WindowProps &props) : m_props(props)
	{
		init();
	}

	void OpenGL::init() {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}


	void OpenGL::render()
	{


		/*
			m_editorFramebuffer->bind();
			m_currentScene->editorUpdate(m_editorCamera);
			m_editorFramebuffer->unbind();

			m_gameFramebuffer->bind();
			m_currentScene->renderScene();
			m_gameFramebuffer->unbind();
		*/
		
		// loop through unordered_map of viewports
		for (auto& [ID, viewport] : m_viewports) {

			// if viewport doesnt have any scene
			if (!viewport.scene) {
				continue;
			}

			viewport.framebuffer->bind();
			
			ResourceManager& resourceManager = Application::getApplication().getResourceManager();
			clear();
			glUseProgram(m_defaultShader.ID);
			setViewport(0, 0, viewport.width, viewport.height);
			auto view = viewport.scene->getRegistry().view<const MeshComponent, TransformComponent>(); // register of all mesh components
			auto cameraView = viewport.scene->getRegistry().view<CameraComponent>();
			for (const auto cameraEntity : cameraView) { // change this we are currently rendering multiple cameras

				for (const auto entity : view) {
					auto& meshComponent = view.get<MeshComponent>(entity);
					auto& transform = view.get<TransformComponent>(entity);

					auto& current_camera = cameraView.get<CameraComponent>(cameraEntity);
					current_camera.m_viewPortX = viewport.width;
					current_camera.m_viewPortY = viewport.height;

					if (current_camera.isActive()) {
						glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.ID, "model"), 1, GL_FALSE, &transform.model[0][0]);
						glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.ID, "view"), 1, GL_FALSE, &current_camera.getView()[0][0]);
						glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.ID, "proj"), 1, GL_FALSE, &transform.model[0][0]);

						glUniform3fv(glGetUniformLocation(m_defaultShader.ID, "cameraPos"), 1, &current_camera.getPos()[0]);
						for (unsigned int id : meshComponent.meshIDs) {
							resourceManager.m_loadedMeshes[id].draw(m_defaultShader);
						}

					}

				}
			}

			viewport.framebuffer->unbind();
		}
	}

	void OpenGL::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	}

	void OpenGL::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	uint32_t OpenGL::createViewport(Scene* scene, ViewportType type)
	{
		uint32_t viewportID = nextViewportID;
		m_viewports.emplace(std::piecewise_construct, std::forward_as_tuple(viewportID), std::forward_as_tuple(1920,1080));
		Viewport& tempViewport = m_viewports.at(viewportID);
		tempViewport.type = type;
		tempViewport.scene = scene;
		nextViewportID++;
		return viewportID;
	}

	Viewport& OpenGL::getViewport(uint32_t viewportID)
	{
		return m_viewports.find(viewportID)->second;
	}

}