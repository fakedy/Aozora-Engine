#pragma once
#include <luau/lua.h>
#include <luau/lualib.h>
#include <iostream>
#include <unordered_map>
#include <entt/entt.hpp>
namespace Aozora {


	class ScriptSystem {
	public:


		struct ScriptInstance {
			lua_State* thread;
		};

		ScriptSystem() {

			L = luaL_newstate();

			luaL_openlibs(L);
			std::cout << "ScriptSystem initialized" << std::endl;
		}
		virtual ~ScriptSystem() = default;

		void update(entt::registry& registry);


	private:

		std::unordered_map<uint32_t, ScriptInstance> m_scriptInstances; // Map of script instances by entity ID
		lua_State* L = nullptr; // Lua state pointer

	};

	








}