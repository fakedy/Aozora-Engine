#pragma once
#include <Systems/Events/EventSystem.h>


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

	class createProjectRequest : public Event {

	public:

		createProjectRequest() : Event(EventType::ProjectCreated) {}

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
}
