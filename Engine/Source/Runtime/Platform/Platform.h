#pragma once

#if defined(WI_PLATFORM_WIN)
	#include "Windows/WindowsPlatform.h"
	#include "Windows/WindowsApplication.h"
#elif defined(WI_PLATFORM_ANDROID)
	#include "Android/AndroidPlatform.h"
	#include "Android/AndroidApplication.h"
#elif defined(WI_PLATFORM_LINUX)
	#include "Linux/LinuxPlatform.h"
	#include "Linux/LinuxApplication.h"
#endif
