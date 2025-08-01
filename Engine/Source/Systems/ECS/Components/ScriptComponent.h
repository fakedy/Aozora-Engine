#pragma once
#include <string>




namespace Aozora {


	struct ScriptComponent {
		uint32_t scriptID; // ID of the script to be executed
		bool isActive; // Whether the script is active or not
		std::string name;
	};
}