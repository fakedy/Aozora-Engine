#pragma once
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


// to be honest I dont think ill ever use anything other than GLFW, and from what I know GLFW is also available on linux

namespace Aozora {
	class WindowsWindow : public Window {
	public:

		WindowsWindow(WindowProps& props);
		~WindowsWindow();

		void onUpdate() override;
		void shutdown() override;
		bool windowShouldClose() override;

		void* getNativeWindow() override;

		


	private:

		void init();

		GLFWwindow* m_window;

		WindowProps& m_props;

	};




}