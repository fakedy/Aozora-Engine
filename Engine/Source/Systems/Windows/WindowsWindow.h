#pragma once
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


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