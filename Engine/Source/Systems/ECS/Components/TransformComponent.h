#pragma once
#include <glm/glm.hpp>


namespace Aozora {

	struct TransformComponent {
		// can prob just use one matrix :)
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 rot = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
	};
}
