#pragma once
#include <GLFW/glfw3.h>

#include "Wismut/Core/Events/Event.h"

#ifdef WI_PLATFORM_WIN
#include "Wismut/Core/Window.h"

namespace Wi
{
	class WindowsWindow : public Window
	{
		struct WindowData
		{
			int Width;
			int Height;
			EventCallback EventCallback;
		};
	public:
		WindowsWindow(const WindowSpecification& specification);

		void PollEvents() override;
		void Destroy() override;
		void* GetNativeWindow() override { return m_Window; }

	private:
		GLFWwindow* m_Window;
		WindowData m_Data;
	};
}
#endif
