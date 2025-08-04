#pragma once
#include <glm/glm.hpp>
#include <cereal/cereal.hpp>
#include <Systems/Serialization/SerializationGLM.h>

namespace Aozora {

	struct TransformComponent {
		glm::mat4 model = glm::mat4(1.0f); // mat4 storage of the pos/rot/scale
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 rot = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);

		bool isDirty{ true };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(model),
				CEREAL_NVP(pos),
				CEREAL_NVP(rot),
				CEREAL_NVP(scale),
				CEREAL_NVP(isDirty));
		}
	};
}
