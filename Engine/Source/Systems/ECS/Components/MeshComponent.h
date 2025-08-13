#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <cereal/cereal.hpp>

namespace Aozora {

	struct MeshComponent {
	public:
		bool m_isActive{ true };
		uint64_t meshID{0};
		uint64_t materialID{ 0 };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(m_isActive),
				CEREAL_NVP(meshID),
				CEREAL_NVP(materialID));
		}

	};

}