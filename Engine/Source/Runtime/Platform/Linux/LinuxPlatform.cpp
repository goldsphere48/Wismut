#ifdef WI_PLATFORM_LINUX
#include "LinuxPlatform.h"
#include "Platform/IPlatform.h"
#include "LinuxApplication.h"

namespace Wi::Platform
{
	UniquePtr<IPlatformApplication> CreateNativeApplication()
	{
		return CreateUniquePtr<LinuxApplication>();
	}
}
#endif
