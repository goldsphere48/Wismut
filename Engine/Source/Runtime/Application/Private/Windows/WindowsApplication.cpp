#include "Windows/WindowsApplication.h"
#include <Windows.h>

HINSTANCE hInstance;

namespace Wi
{
	WindowsApplication::WindowsApplication()
	{
		hInstance = GetModuleHandle(nullptr);
	}

	WindowsApplication::~WindowsApplication()
	{
		
	}
}
