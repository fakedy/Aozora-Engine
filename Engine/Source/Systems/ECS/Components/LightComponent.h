#pragma once
#include <glm/glm.hpp>


namespace Aozora {

	enum LightType {
		Direction,
		Area,
		Point
	};

	struct LightComponent {

		LightType type{ Area };
		glm::fvec3 color{ 1.0f };
		float radius{ 5.0f };
		float linear{ 0.7f };
		float quadratic{ 1.8f };
		float power{ 1.0f };
	};
}
