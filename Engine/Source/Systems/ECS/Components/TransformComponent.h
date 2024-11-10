#pragma once
#include <glm/glm.hpp>

namespace Aozora {

	struct Transform {
		// can prob just use a matrix :)
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};
}
