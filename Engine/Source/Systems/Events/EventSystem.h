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
		loadSceneRequest,
		saveSceneRequest,
		ViewportResize,
		NewMesh
	};

	// Base class for events
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

		// alias to shorten the function
		using EventCallbackFunc = std::function<void(Event&)>;

		// will push a subscriber into the list of subscribers of a certain event type
		static void subscribe(EventType type, const EventCallbackFunc& callback) {
			s_listeners[type].push_back(callback);
		}

		static void dispatch(Event* event) {
			assert(event != nullptr && "Attempted to dispatch a null event!");
			m_eventQueue.emplace_back(event);
		}

		// Main reason for the delayed events is because doing a lot of stuff mid frame/update can invalidate pointers and stuff.
		static void flush() {

			// for all events in the queue
			for (const auto& event : m_eventQueue) {

				// if its a valid event
				if (event) {

					// go through all the listeners and call the callback
					EventType type = event->getEventType();
					if (s_listeners.find(type) != s_listeners.end()) {
						for (auto& callback : s_listeners[type]) {
							callback(*event);
						}
					}
				}
			}
			// since we've processed the queue we clear it
			m_eventQueue.clear();
		}

		
	private:

		inline static std::map<EventType, std::vector<EventCallbackFunc>> s_listeners;
		inline static std::vector<std::unique_ptr<Event>> m_eventQueue;

	};


}