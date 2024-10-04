#include "OpenGL.h"
#include <iostream>
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/MeshComponent.h"


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


	}

	void OpenGL::render()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.7f, 0.0f, 1.0f);

		auto view = m_registry->view<const meshComponent>();

		for (const auto entity : view) {
			auto& mesh = view.get<meshComponent>(entity);
			glDrawElements(GL_TRIANGLES, mesh.mesh->indicesSize, GL_UNSIGNED_INT, 0);
			// do something
		}



	}

}