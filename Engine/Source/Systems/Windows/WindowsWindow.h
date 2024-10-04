#pragma once
#include "Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace Aozora {
	class WindowsWindow : public Window {
	public:

		WindowsWindow(const WindowProps& props);
		~WindowsWindow();

		void onUpdate() override;
		void shutdown() override;
		bool windowShouldClose() override;

		void* getNativeWindow() override;

	private:

		void init(const WindowProps& props);

		GLFWwindow* m_window;

	};




}