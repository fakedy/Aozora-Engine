#pragma once
#include "Renderer.h"
#include "glad/glad.h"
#include "Opengl/OpenglShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Systems/Windows/Window.h"
#include <unordered_map>
#include "Opengl/OpenglFrameBuffer.h"
#include "Viewport.h"



namespace Aozora {

	class OpenGL : public Renderer
	{
	public:

		OpenGL(Window::WindowProps &props);

		void render() override;
		void clear() override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		uint32_t createViewport(Scene* scene, ViewportType type);

		Viewport& getViewport(uint32_t viewportID);

		virtual void updatePrimaryScene(Scene* scene) override;

	private:
		Window::WindowProps& m_props;

		std::unordered_map<uint32_t, Viewport> m_viewports;
		uint32_t nextViewportID{ 0 };

		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
		OpenglShader m_gridShader{ OpenglShader("Resources/Shaders/v_grid.glsl", "Resources/Shaders/f_grid.glsl") };
	
		void init();
	};
}


