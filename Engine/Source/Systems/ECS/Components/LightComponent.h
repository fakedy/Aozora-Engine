#pragma once
#include <glm/glm.hpp>


namespace Aozora {

	enum LightType {
		Direction,
		Area,
		Point
	};

	struct LightComponent {

		LightType type{ Direction };
		glm::fvec3 color{ 1.0f };




	};
}
