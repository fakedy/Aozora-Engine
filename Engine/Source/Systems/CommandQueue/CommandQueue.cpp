#include "CommandQueue.h"


namespace Aozora {


	void CommandQueue::queueAction(std::function<void()> func)
	{
		m_actionQueue.push(func);
	}
	void CommandQueue::processActions()
	{
		while (!m_actionQueue.empty()) {
			m_actionQueue.front()();
			m_actionQueue.pop();
		}
	}

}