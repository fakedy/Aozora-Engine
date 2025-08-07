#pragma once
#include <vector>
#include <map>
#include <functional>
#include <cassert>



namespace Aozora {


	enum class EventType {
		None = 0,
		ProjectCreated,
		SceneCreated,
		ChangeScene,
		CreateProjectRequest,
		SaveProjectRequest,
		ViewportResize,
		NewMesh
	};

	class Event {
	public:

		virtual ~Event() = default;
		virtual 
			EventType getEventType() const {
			return m_type;
		}


	protected:

		Event(EventType type) : m_type(type) {}
		EventType m_type;
	};

	class IEventListener {
	public:
		virtual ~IEventListener() = default;
		virtual void onEvent(Event& e) = 0;
	};



	class EventDispatcher {

	public:

		using EventCallbackFunc = std::function<void(Event&)>;

		static void subscribe(EventType type, const EventCallbackFunc& callback) {
			s_listeners[type].push_back(callback);
		}

		static void dispatch(Event* event) {
			assert(event != nullptr && "Attempted to dispatch a null event!");
			m_eventQueue.emplace_back(event);
		}

		static void flush() {

			for (const auto& event : m_eventQueue) {

				if (event) {

					EventType type = event->getEventType();
					if (s_listeners.find(type) != s_listeners.end()) {
						for (auto& callback : s_listeners[type]) {
							callback(*event);
						}
					}
				}
			}
			m_eventQueue.clear();
		}

		
	private:

		inline static std::map<EventType, std::vector<EventCallbackFunc>> s_listeners;
		inline static std::vector<std::unique_ptr<Event>> m_eventQueue;

	};


}