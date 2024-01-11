#include "wipch.h"

#ifdef WI_PLATFORM_WIN
#include "WindowsWindow.h"
#include <GLFW/glfw3.h>

namespace Wi
{
	WindowsWindow::WindowsWindow(const WindowSpecification& specification)
	{
		const auto result = glfwInit();
		WI_CORE_ASSERT(result, "Failed to initialize glfw");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(specification.Width, specification.Height, specification.Title.c_str(), nullptr, nullptr);
	}

	void WindowsWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void WindowsWindow::Destroy()
	{
		glfwDestroyWindow(m_Window);
	}
}
#endif
