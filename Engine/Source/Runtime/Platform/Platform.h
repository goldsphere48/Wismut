#pragma once

#if defined(WI_PLATFORM_WIN)
	#include "Windows/WindowsPlatform.h"
	#include "Windows/WindowsConsole.h"
	#include "Windows/WindowsTime.h"
	#include "Windows/WindowsMemory.h"
	#include "Windows/WindowsApplication.h"
#elif defined(WI_PLATFORM_LINUX)
	#include "Linux/LinuxPlatform.h"
	#include "Linux/LinuxConsole.h"
	#include "Linux/LinuxTime.h"
	#include "Linux/LinuxMemory.h"
	#include "Linux/LinuxApplication.h"
#endif
