#ifdef WI_PLATFORM_LINUX
#include "Platform/Platform.hpp"
#include "LinuxApplication.hpp"

namespace Wi
{
	UniquePtr<IPlatformApplication> Platform::CreateNativeApplication()
	{
		return CreateUniquePtr<LinuxApplication>();
	}
}
#endif
