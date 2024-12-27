#include "WindowsWindow.h"
#include <iostream>
#include "Systems/Input.h"

// no logger implemented yet


namespace Aozora {

	Window* Window::create(const WindowProps& props) {
		return new WindowsWindow(props); 
	}



	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::onUpdate()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);
	}

	bool WindowsWindow::windowShouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	void* WindowsWindow::getNativeWindow()
	{
		return m_window;
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		// initiate glfw
		if (!glfwInit()) {
			std::cout << "GLFW failed to init\n";
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		m_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_window);

		// using opengl
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Glad failed to load\n";
		}

		glfwSetKeyCallback(m_window, Input::keyCallback);

	}
}
