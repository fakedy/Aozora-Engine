#pragma once
#include <cereal/cereal.hpp>

namespace Aozora {



	struct SkyboxComponent {
	public:
		
		uint64_t id{ 0 };

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(id));
		}
	};
}
