#pragma once


class Scene {
public:
	
	Scene() {
		registry = std::make_shared<entt::registry>();
	}


	const char* m_sceneName{ "New Scene" };

	// ECS SYSTEM
	std::shared_ptr<entt::registry> registry;


private:


};