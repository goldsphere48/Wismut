#include "wipch.h"

#include "Window.h"

#include "Wismut/Platform/Windows/WindowsWindow.h"

namespace Wi
{
	std::shared_ptr<Window> Window::Create(const WindowSpecification& specification)
	{
#ifdef WI_PLATFORM_WIN
		return std::make_shared<WindowsWindow>(specification);
#endif
	}
}
