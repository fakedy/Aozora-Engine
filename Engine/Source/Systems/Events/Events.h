#pragma once
#include <Systems/Events/EventSystem.h>
#include <entt/entt.hpp>


namespace Aozora {
	class Project;
	class Scene;
}

namespace Aozora {

	class ProjectCreatedEvent : public Event {

	public:

		ProjectCreatedEvent(Project& project) : Event(EventType::ProjectCreated), m_project(project){ }


		Project& getProject() const {
			return m_project;
		}
	protected:

		Project& m_project;
	};

	class CreateProjectRequest : public Event {

	public:

		CreateProjectRequest() : Event(EventType::CreateProjectRequest) {}

	protected:

	};

	class SaveProjectRequest : public Event {

	public:

		SaveProjectRequest() : Event(EventType::SaveProjectRequest) {}

	protected:

	};

	class ChangeSceneEvent : public Event {

	public:

		ChangeSceneEvent(Scene& scene) : Event(EventType::SceneCreated), m_scene(scene) {}

		Scene& getScene() const {
			return m_scene;
		}

	protected:

		Scene& m_scene;
	};

	class SaveSceneRquest : public Event {

	public:

		SaveSceneRquest(Scene& scene) : Event(EventType::saveSceneRequest), m_scene(scene) {}

		Scene& getScene() const {
			return m_scene;
		}

	protected:

		Scene& m_scene;
	};

	class LoadSceneRequest : public Event {

	public:

		LoadSceneRequest(uint64_t id) : Event(EventType::loadSceneRequest), id(id) {}

		int getID() const {
			return id;
		}

	protected:

		uint64_t id;
	};

	class viewportResize : public Event {

	public:

		viewportResize(int id, int width, int height ) : Event(EventType::SceneCreated), id(id), width(width), height(height) {}

		int getViewportID() const {
			return id;
		}

	protected:

		int id;
		int width;
		int height;
	};


	class EntityCreatedWithMeshEvent : public Event {

	public:

		EntityCreatedWithMeshEvent(entt::entity entity, uint32_t meshID, Scene* scene) : Event(EventType::NewMesh), m_entity(entity), m_meshID(meshID), m_scene(scene) {}

		entt::entity getEntity() const{
			return m_entity;
		}
		uint32_t getID() const {
			return m_meshID;
		}

		Scene* getScene() const {
			return m_scene;
		}

	protected:

		entt::entity m_entity;
		uint32_t m_meshID{0};
		Scene* m_scene;
	};
}
