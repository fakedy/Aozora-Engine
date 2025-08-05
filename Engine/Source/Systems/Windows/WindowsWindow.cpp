#include "WindowsWindow.h"
#include <iostream>
#include "Systems/Input.h"

// no logger implemented yet


namespace Aozora {

	Window* Window::create(WindowProps& props) {
		return new WindowsWindow(props); 
	}

	WindowsWindow::WindowsWindow(WindowProps& props) : m_props(props)
	{
		init();
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::onUpdate()
	{
		glfwSwapBuffers(m_window);
		glfwGetFramebufferSize(m_window, &m_props.width, &m_props.height);
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

	void WindowsWindow::init()
	{
		// initiate glfw
		if (!glfwInit()) {
			std::cout << "GLFW failed to init\n";
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		m_window = glfwCreateWindow(m_props.width, m_props.height, m_props.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_window);

		// using opengl
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Glad failed to load\n";
		}

		glfwSetKeyCallback(m_window, Input::keyCallback);
		glfwSetCursorPosCallback(m_window, Input::mouseCallback);
		glfwSetMouseButtonCallback(m_window, Input::mouseButtonCallback);

		glfwSwapInterval(0);

	}


}
