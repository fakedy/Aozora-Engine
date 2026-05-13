#pragma once
#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <Systems/Serialization/SerializationGLM.h>

namespace Aozora {

	struct TransformComponent {
		glm::dmat4 model = glm::dmat4(1.0f); // mat4 storage of the pos/rot/scale
		glm::dmat4 baseModel = glm::dmat4(1.0f);
		glm::dvec3 pos = glm::dvec3(0.0f);
		glm::dvec3 rot = glm::dvec3(0.0f);
		glm::dvec3 scale = glm::dvec3(1.0f);

		bool isDirty{ true };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(model),	
				CEREAL_NVP(baseModel),
				CEREAL_NVP(pos),
				CEREAL_NVP(rot),
				CEREAL_NVP(scale),
				CEREAL_NVP(isDirty));
		}
	};
}
