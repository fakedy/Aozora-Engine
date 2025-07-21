#pragma once


#include <vector>
#include <map>
#include <functional>



namespace Aozora {


	enum class EventType {
		None = 0,
		ProjectCreated,
		SceneCreated,
		ChangeScene,
	};

	class Event {
	public:

		virtual 
		EventType getEvent() const {
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

		static void dispatch(Event& event) {
			EventType type = event.getEvent();
			if (s_listeners.find(type) != s_listeners.end()) {
				for (auto& callback : s_listeners[type]) {
					callback(event);
				}
			}
		}
		
	private:

		inline static std::map<EventType, std::vector<EventCallbackFunc>> s_listeners;

	};


}