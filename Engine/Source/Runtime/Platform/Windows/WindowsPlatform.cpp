#ifdef WI_PLATFORM_WIN
#include "Platform/Platform.hpp"
#include "WindowsApplication.hpp"

namespace Wi
{
	UniquePtr<IPlatformApplication> Platform::CreateNativeApplication()
	{
		return CreateUniquePtr<WindowsApplication>();
	}
}
#endif
