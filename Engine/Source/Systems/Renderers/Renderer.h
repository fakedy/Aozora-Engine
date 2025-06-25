#pragma once
#include "Systems/Windows/Window.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include <Systems/Scene/Scene.h>

namespace Aozora {

	class Renderer
	{
	public:



		virtual void render() = 0;
		virtual void clear() = 0;

		static Renderer* create(Window::WindowProps &props);

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void updatePrimaryScene(Scene* scene) = 0;

	private:

	protected:

	};
}
