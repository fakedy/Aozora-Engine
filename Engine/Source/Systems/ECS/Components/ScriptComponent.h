#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>


namespace Aozora {


	struct ScriptComponent {
	public:
		uint32_t scriptID; // ID of the script to be executed
		bool isActive; // Whether the script is active or not
		std::string name;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(scriptID),
				CEREAL_NVP(isActive),
				CEREAL_NVP(name));
		}
	};
}