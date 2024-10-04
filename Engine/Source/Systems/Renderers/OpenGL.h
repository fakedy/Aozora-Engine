#pragma once
#include "Renderer.h"
#include "glad/glad.h"

namespace Aozora {

	class OpenGL : public Renderer
	{
	public:

		OpenGL(std::shared_ptr<entt::registry> registry);



		void render() override;


	private:
		void init();
		std::shared_ptr<entt::registry> m_registry;
	};
}


