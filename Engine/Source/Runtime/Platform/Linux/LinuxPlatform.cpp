#ifdef WI_PLATFORM_LINUX
#include "Platform/Platform.hpp"
#include "Application/Linux/LinuxApplication.hpp"

namespace Wi
{
	UniquePtr<INativeApplication> Platform::CreateNativeApplication()
	{
		return CreateUniquePtr<LinuxApplication>();
	}
}
#endif
