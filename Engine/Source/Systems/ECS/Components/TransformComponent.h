#pragma once
#include <glm/glm.hpp>


namespace Aozora {

	struct Transform {
		// can prob just use a matrix :)
		glm::mat4 model = glm::mat4(1.0f);
	};
}
