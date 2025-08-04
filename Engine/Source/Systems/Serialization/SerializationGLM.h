#pragma once
#include <cereal/cereal.hpp>
#include <glm/glm.hpp>

namespace cereal { // will not compile unless we put it in cereal namespace, it cant find these functions otherwise

	// Because cereal doesnt know how to handle glm we have to provide these functions
	template <class Archive>
	void serialize(Archive& archive, glm::vec2& v) {
		archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
	}
	template <class Archive>
	void serialize(Archive& archive, glm::vec3& v) {
		archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z));
	}
	template <class Archive>
	void serialize(Archive& archive, glm::vec4& v) {
		archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y), CEREAL_NVP(v.z), CEREAL_NVP(v.w));
	}
	template <class Archive>
	void serialize(Archive& archive, glm::mat4& m) {
		archive(CEREAL_NVP(m[0]), CEREAL_NVP(m[1]), CEREAL_NVP(m[2]), CEREAL_NVP(m[3]));
	}
}