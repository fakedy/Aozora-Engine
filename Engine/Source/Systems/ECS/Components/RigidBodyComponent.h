#pragma once
#include <cereal/cereal.hpp>

namespace Aozora {

	struct RigidBodyComponent {
	public:
		template<class Archive>
		void serialize(Archive& archive) {
		}
	};



}