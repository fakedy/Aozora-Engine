#pragma once
#include <functional>
#include <queue>



namespace Aozora {


	class CommandQueue {
	public:
		void queueAction(std::function<void()> func);
		void processActions();


	private:



		std::queue<std::function<void()>> m_actionQueue;
	};


}