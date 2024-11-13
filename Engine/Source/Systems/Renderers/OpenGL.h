#pragma once
#include "Renderer.h"
#include "glad/glad.h"
#include "Opengl/OpenglShader.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Aozora {

	class OpenGL : public Renderer
	{
	public:

		OpenGL(std::shared_ptr<entt::registry> registry);

		void render() override;


	private:
		void init();
		std::shared_ptr<entt::registry> m_registry;

		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
	};
}


