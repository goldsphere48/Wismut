#ifdef WI_PLATFORM_WIN
#include "WindowsPlatform.h"
#include "Platform/IPlatform.h"
#include "WindowsApplication.h"

namespace Wi::Platform
{
	UniquePtr<IPlatformApplication> CreateNativeApplication()
	{
		return CreateUniquePtr<WindowsApplication>();
	}
}
#endif
