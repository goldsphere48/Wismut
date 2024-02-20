#include "wipch.h"

#include "Wismut/Core/Input.h"
#include "Wismut/Core/Events/KeyEvent.h"
#include "Wismut/Core/Events/MouseEvents.h"
#include "Wismut/Core/Events/WindowEvents.h"
#include "Wismut/Renderer/Renderer.h"

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

		const int width = static_cast<int>(specification.Width);
		const int height = static_cast<int>(specification.Height);

		m_Window = glfwCreateWindow(width, height, specification.Title.c_str(), nullptr, nullptr);

		m_Data = WindowData
		{
			.Width = width,
			.Height = height,
			.EventCallback = specification.EventCallback
		};

		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			const auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent e;
			data->EventCallback(e);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowResizeEvent e(width, height);
			data->Width = width;
			data->Height = height;
			data->EventCallback(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			const auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					Input::UpdateKeyState(static_cast<KeyCode>(key), KeyState::Pressed);
					KeyPressedEvent e(static_cast<KeyCode>(key), false);
					data->EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					Input::UpdateKeyState(static_cast<KeyCode>(key), KeyState::Released);
					KeyReleasedEvent e(static_cast<KeyCode>(key));
					data->EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					Input::UpdateKeyState(static_cast<KeyCode>(key), KeyState::Held);
					KeyPressedEvent e(static_cast<KeyCode>(key), true);
					data->EventCallback(e);
					break;
				}
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
		{
			const auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMovedEvent e(x, y);
			data->EventCallback(e);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			const auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:
				{
					Input::UpdateButtonState(static_cast<MouseButton>(button), KeyState::Pressed);
					MouseButtonPressedEvent e(static_cast<MouseButton>(button));
					data->EventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					Input::UpdateButtonState(static_cast<MouseButton>(button), KeyState::Released);
					MouseButtonReleasedEvent e(static_cast<MouseButton>(button));
					data->EventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					Input::UpdateButtonState(static_cast<MouseButton>(button), KeyState::Held);
					MouseButtonPressedEvent e(static_cast<MouseButton>(button));
					data->EventCallback(e);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			const auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseScrolledEvent e(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data->EventCallback(e);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
		{
			const auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			MouseMovedEvent e(static_cast<float>(x), static_cast<float>(y));
			data->EventCallback(e);
			Input::UpdateMousePosition(x, y);
		});

		glfwSetErrorCallback([](int error_code, const char* description)
		{
			WI_CORE_ERROR("[GLFW] Error: {0} {1}", error_code, description);
		});
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
