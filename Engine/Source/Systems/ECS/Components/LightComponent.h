#pragma once
#include <glm/glm.hpp>
#include <cereal/cereal.hpp> 
#include <Systems/Serialization/SerializationGLM.h>

namespace Aozora {


	enum LightType {
		Direction,
		Area,
		Point
	};

	struct LightComponent {
	public:
		LightType type{ Area };
		glm::fvec3 color{ 1.0f };
		float radius{ 5.0f };
		float linear{ 0.7f };
		float quadratic{ 1.8f };
		float power{ 1.0f };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(type),
				CEREAL_NVP(color),
				CEREAL_NVP(radius),
				CEREAL_NVP(linear),
				CEREAL_NVP(quadratic),
				CEREAL_NVP(power));
		}
	};
}
