#pragma once
#include <GLFW/glfw3.h>

#ifdef WI_PLATFORM_WIN
#include "Wismut/Core/Window.h"

namespace Wi
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& specification);

		void PollEvents() override;
		void Destroy() override;

	private:
		GLFWwindow* m_Window;
	};
}
#endif
