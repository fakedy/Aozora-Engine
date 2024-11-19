#pragma once
#include "Systems/Windows/Window.h"
#include <glm/glm.hpp>

namespace Aozora {

	class Renderer
	{
	public:


		virtual void render(glm::mat4 model, glm::mat4 view, glm::mat4 proj) = 0;

		static Renderer* create(Window::WindowProps props);

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	private:

	protected:
		Window::WindowProps m_props;

	};
}
