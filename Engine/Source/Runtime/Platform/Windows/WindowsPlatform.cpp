#ifdef WI_PLATFORM_WIN
#include "Application/Windows/WindowsApplication.hpp"
#include "Platform/Platform.hpp"

namespace Wi
{
	UniquePtr<INativeApplication> Platform::CreateNativeApplication()
	{
		return CreateUniquePtr<WindowsApplication>();
	}
}
#endif
